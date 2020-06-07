//
// Created by MasterLogick on 3/8/20.
//
#include <alext.h>
#include <iostream>
#include <chrono>
#include <algorithm>
#include "al.h"
#include "alc.h"
#include "AudioState.h"
#include "../audio/AudioSystem.h"

extern "C" {
#include <libavutil/time.h>
}

namespace osu {
    static std::chrono::microseconds get_avtime() { return std::chrono::microseconds{av_gettime()}; }

    // Per-buffer size, in time
    const std::chrono::milliseconds AudioBufferTime{20};
// Buffer total size, in time (should be divisible by the buffer time)
    const std::chrono::milliseconds AudioBufferTotalTime{800};

    AudioState::~AudioState() {
        if (mSource)
            alDeleteSources(1, &mSource);
        if (!mBuffers.empty())
            alDeleteBuffers(static_cast<ALsizei>(mBuffers.size()), mBuffers.data());

        av_freep(&mSamples);
    }

    std::chrono::nanoseconds AudioState::getClock() {
        std::lock_guard<std::mutex> lock{mSrcMutex};
        return getClockNoLock();
    }

    std::chrono::nanoseconds AudioState::getClockNoLock() {
        // The audio clock is the timestamp of the sample currently being heard.
        if (AudioSystem::alcGetInteger64vSOFT) {
            // If device start time = min, we aren't playing yet.
            if (mDeviceStartTime == std::chrono::nanoseconds::min())
                return std::chrono::nanoseconds::zero();

            // Get the current device clock time and latency.
            auto device = alcGetContextsDevice(alcGetCurrentContext());
            ALCint64SOFT devtimes[2]{0, 0};
            AudioSystem::alcGetInteger64vSOFT(device, ALC_DEVICE_CLOCK_LATENCY_SOFT, 2, devtimes);
            auto latency = std::chrono::nanoseconds{devtimes[1]};
            auto device_time = std::chrono::nanoseconds{devtimes[0]};

            // The clock is simply the current device time relative to the recorded
            // start time. We can also subtract the latency to get more a accurate
            // position of where the audio device actually is in the output stream.
            return device_time - mDeviceStartTime - latency;
        }

        // The source-based clock is based on 4 components:
//         * 1 - The timestamp of the next sample to buffer (mCurrentPts)
//         * 2 - The length of the source's buffer queue
//         *     (AudioBufferTime*AL_BUFFERS_QUEUED)
//         * 3 - The offset OpenAL is currently at in the source (the first value
//         *     from AL_SAMPLE_OFFSET_LATENCY_SOFT)
//         * 4 - The latency between OpenAL and the DAC (the second value from
//         *     AL_SAMPLE_OFFSET_LATENCY_SOFT)
//         *
//         * Subtracting the length of the source queue from the next sample's
//         * timestamp gives the timestamp of the sample at the start of the source
//         * queue. Adding the source offset to that results in the timestamp for the
//         * sample at OpenAL's current position, and subtracting the source latency
//         * from that gives the timestamp of the sample currently at the DAC.

        std::chrono::nanoseconds pts{mCurrentPts};
        if (mSource) {
            ALint64SOFT offset[2];

            // NOTE: The source state must be checked last, in case an underrun
            //    * occurs and the source stops between retrieving the offset+latency
            //    * and getting the state.
            if (AudioSystem::alGetSourcei64vSOFT) {
                AudioSystem::alGetSourcei64vSOFT(mSource, AL_SAMPLE_OFFSET_LATENCY_SOFT, offset);
            } else {
                ALint ioffset;
                alGetSourcei(mSource, AL_SAMPLE_OFFSET, &ioffset);
                offset[0] = ALint64SOFT{ioffset} << 32;
                offset[1] = 0;
            }
            ALint queued, status;
            alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);
            alGetSourcei(mSource, AL_SOURCE_STATE, &status);

            // If the source is AL_STOPPED, then there was an underrun and all
            //* buffers are processed, so ignore the source queue. The audio thread
            //   * will put the source into an AL_INITIAL state and clear the queue
            //     * when it starts recovery.
            if (status != AL_STOPPED) {
                pts -= AudioBufferTime * queued;
                pts += std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::duration<int64_t, std::ratio<1, (((long long int) 1) << 32)>>{
                                offset[0] / mCodecCtx->sample_rate});
            }
            // Don't offset by the latency if the source isn't playing.
            if (status == AL_PLAYING)
                pts -= std::chrono::nanoseconds{offset[1]};
        }

        return std::max(pts, std::chrono::nanoseconds::zero());
    }

    void AudioState::startPlayback() {
        alSourcePlay(mSource);
        if (AudioSystem::alcGetInteger64vSOFT) {
            // Subtract the total buffer queue time from the current pts to get the
            // pts of the start of the queue.
            std::chrono::nanoseconds startpts{mCurrentPts - AudioBufferTotalTime};
            int64_t srctimes[2]{0, 0};
            AudioSystem::alGetSourcei64vSOFT(mSource, AL_SAMPLE_OFFSET_CLOCK_SOFT, srctimes);
            auto device_time = std::chrono::nanoseconds{srctimes[1]};
            auto src_offset = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::duration<int64_t, std::ratio<1, ((int64_t) 1 << 32)>>{srctimes[0]}) /
                              mCodecCtx->sample_rate;

            // The mixer may have ticked and incremented the device time and sample
            // offset, so subtract the source offset from the device time to get
            // the device time the source started at. Also subtract startpts to get
            // the device time the stream would have started at to reach where it
            // is now.
            mDeviceStartTime = device_time - src_offset - startpts;
        }
    }

    int AudioState::getSync() {
        return 0;
    }

    int AudioState::decodeFrame() {
        while (!mMovie->mQuit) {
            int ret;
            while ((ret = avcodec_receive_frame(mCodecCtx, mDecodedFrame)) == AVERROR(EAGAIN))
                mPackets.sendTo(mCodecCtx);
            if (ret != 0) {
                if (ret == AVERROR_EOF) break;
                std::cerr << "Failed to receive frame: " << ret << std::endl;
                continue;
            }

            if (mDecodedFrame->nb_samples <= 0)
                continue;

            // If provided, update w/ pts
            if (mDecodedFrame->best_effort_timestamp != AV_NOPTS_VALUE)
                mCurrentPts = std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::duration<double>{av_q2d(mStream->time_base) * mDecodedFrame->best_effort_timestamp}
                );

            if (mDecodedFrame->nb_samples > mSamplesMax) {
                av_freep(&mSamples);
                av_samples_alloc(
                        &mSamples, nullptr, mCodecCtx->channels,
                        mDecodedFrame->nb_samples, mDstSampleFmt, 0
                );
                mSamplesMax = mDecodedFrame->nb_samples;
            }
            // Return the amount of sample frames converted
            int data_size{swr_convert(mSwresCtx, &mSamples, mDecodedFrame->nb_samples,
                                      const_cast<const uint8_t **>(mDecodedFrame->data), mDecodedFrame->nb_samples)};

            av_frame_unref(mDecodedFrame);
            return data_size;
        }

        return 0;
    }

// Duplicates the sample at in to out, count times. The frame size is a
// * multiple of the template type size.

    template<typename T>
    static void sample_dup(uint8_t *out, const uint8_t *in, unsigned int count, size_t frame_size) {
        auto *sample = reinterpret_cast<const T *>(in);
        auto *dst = reinterpret_cast<T *>(out);
        if (frame_size == sizeof(T))
            std::fill_n(dst, count, *sample);
        else {
            // NOTE: frame_size is a multiple of sizeof(T).
            size_t type_mult{frame_size / sizeof(T)};
            size_t i{0};
            std::generate_n(dst, count * type_mult,
                            [sample, type_mult, &i]() -> T {
                                T ret = sample[i];
                                i = (i + 1) % type_mult;
                                return ret;
                            }
            );
        }
    }


    bool AudioState::readAudio(uint8_t *samples, unsigned int length, int *sample_skip) {
        unsigned int audio_size{0};

        // Read the next chunk of data, refill the buffer, and queue it
//         * on the source
        length /= mFrameSize;
        while (audio_size < length) {
            if (mSamplesLen <= 0 || mSamplesPos >= mSamplesLen) {
                int frame_len = decodeFrame();
                if (frame_len <= 0) break;

                mSamplesLen = frame_len;
                mSamplesPos = std::min(mSamplesLen, *sample_skip);
                *sample_skip -= mSamplesPos;

                // Adjust the device start time and current pts by the amount we're
                // skipping/duplicating, so that the clock remains correct for the
                // current stream position.
                auto skip = std::chrono::nanoseconds{std::chrono::seconds{mSamplesPos}} / mCodecCtx->sample_rate;
                mDeviceStartTime -= skip;
                mCurrentPts += skip;
                continue;
            }

            unsigned int rem{length - audio_size};
            if (mSamplesPos >= 0) {
                const auto len = static_cast<unsigned int>(mSamplesLen - mSamplesPos);
                if (rem > len) rem = len;
                std::copy_n(mSamples + static_cast<unsigned int>(mSamplesPos) * mFrameSize,
                            rem * mFrameSize, samples);
            } else {
                rem = std::min(rem, static_cast<unsigned int>(-mSamplesPos));

                // Add samples by copying the first sample
                if ((mFrameSize & 7) == 0)
                    sample_dup<uint64_t>(samples, mSamples, rem, mFrameSize);
                else if ((mFrameSize & 3) == 0)
                    sample_dup<uint32_t>(samples, mSamples, rem, mFrameSize);
                else if ((mFrameSize & 1) == 0)
                    sample_dup<uint16_t>(samples, mSamples, rem, mFrameSize);
                else
                    sample_dup<uint8_t>(samples, mSamples, rem, mFrameSize);
            }

            mSamplesPos += rem;
            mCurrentPts += std::chrono::nanoseconds{std::chrono::seconds{rem}} / mCodecCtx->sample_rate;
            samples += rem * mFrameSize;
            audio_size += rem;
        }
        if (audio_size <= 0)
            return false;

        if (audio_size < length) {
            const unsigned int rem{length - audio_size};
            std::fill_n(samples, rem * mFrameSize,
                        (mDstSampleFmt == AV_SAMPLE_FMT_U8) ? 0x80 : 0x00);
            mCurrentPts += std::chrono::nanoseconds{std::chrono::seconds{rem}} / mCodecCtx->sample_rate;
            audio_size += rem;
        }
        return true;
    }

    int AudioState::handler() {
        std::unique_lock<std::mutex> srclock{mSrcMutex, std::defer_lock};
        std::chrono::milliseconds sleep_time{AudioBufferTime / 3};
        ALenum fmt;
#ifdef AL_SOFT_bformat_ex
        const bool has_bfmt_ex{alIsExtensionPresent("AL_SOFT_bformat_ex") != AL_FALSE};
        ALenum ambi_layout{AL_FUMA_SOFT};
        ALenum ambi_scale{AL_FUMA_SOFT};
#endif

        // Find a suitable format for OpenAL.
        mDstChanLayout = 0;
        mFormat = AL_NONE;
        if ((mCodecCtx->sample_fmt == AV_SAMPLE_FMT_FLT || mCodecCtx->sample_fmt == AV_SAMPLE_FMT_FLTP) &&
            alIsExtensionPresent("AL_EXT_FLOAT32")) {
            mDstSampleFmt = AV_SAMPLE_FMT_FLT;
            mFrameSize = 4;
            if (mCodecCtx->channel_layout == AV_CH_LAYOUT_7POINT1 &&
                alIsExtensionPresent("AL_EXT_MCFORMATS") &&
                (fmt = alGetEnumValue("AL_FORMAT_71CHN32")) != AL_NONE && fmt != -1) {
                mDstChanLayout = mCodecCtx->channel_layout;
                mFrameSize *= 8;
                mFormat = fmt;
            }
            if ((mCodecCtx->channel_layout == AV_CH_LAYOUT_5POINT1 ||
                 mCodecCtx->channel_layout == AV_CH_LAYOUT_5POINT1_BACK) &&
                alIsExtensionPresent("AL_EXT_MCFORMATS") &&
                (fmt = alGetEnumValue("AL_FORMAT_51CHN32")) != AL_NONE && fmt != -1) {
                mDstChanLayout = mCodecCtx->channel_layout;
                mFrameSize *= 6;
                mFormat = fmt;
            }
            if (mCodecCtx->channel_layout == AV_CH_LAYOUT_MONO) {
                mDstChanLayout = mCodecCtx->channel_layout;
                mFrameSize *= 1;
                mFormat = AL_FORMAT_MONO_FLOAT32;
            }
            // Assume 3D B-Format (ambisonics) if the channel layout is blank and
//             * there's 4 or more channels. FFmpeg/libavcodec otherwise seems to
//             * have no way to specify if the source is actually B-Format (let alone
//             * if it's 2D or 3D).

            if (mCodecCtx->channel_layout == 0 && mCodecCtx->channels >= 4 &&
                alIsExtensionPresent("AL_EXT_BFORMAT") &&
                (fmt = alGetEnumValue("AL_FORMAT_BFORMAT3D_FLOAT32")) != AL_NONE && fmt != -1) {
                int order{static_cast<int>(std::sqrt(mCodecCtx->channels)) - 1};
                if ((order + 1) * (order + 1) == mCodecCtx->channels ||
                    (order + 1) * (order + 1) + 2 == mCodecCtx->channels) {
                    // OpenAL only supports first-order with AL_EXT_BFORMAT, which
//                     * is 4 channels for 3D buffers.

                    mFrameSize *= 4;
                    mFormat = fmt;
                }
            }
            if (!mFormat) {
                mDstChanLayout = AV_CH_LAYOUT_STEREO;
                mFrameSize *= 2;
                mFormat = AL_FORMAT_STEREO_FLOAT32;
            }
        }
        if (mCodecCtx->sample_fmt == AV_SAMPLE_FMT_U8 || mCodecCtx->sample_fmt == AV_SAMPLE_FMT_U8P) {
            mDstSampleFmt = AV_SAMPLE_FMT_U8;
            mFrameSize = 1;
            if (mCodecCtx->channel_layout == AV_CH_LAYOUT_7POINT1 &&
                alIsExtensionPresent("AL_EXT_MCFORMATS") &&
                (fmt = alGetEnumValue("AL_FORMAT_71CHN8")) != AL_NONE && fmt != -1) {
                mDstChanLayout = mCodecCtx->channel_layout;
                mFrameSize *= 8;
                mFormat = fmt;
            }
            if ((mCodecCtx->channel_layout == AV_CH_LAYOUT_5POINT1 ||
                 mCodecCtx->channel_layout == AV_CH_LAYOUT_5POINT1_BACK) &&
                alIsExtensionPresent("AL_EXT_MCFORMATS") &&
                (fmt = alGetEnumValue("AL_FORMAT_51CHN8")) != AL_NONE && fmt != -1) {
                mDstChanLayout = mCodecCtx->channel_layout;
                mFrameSize *= 6;
                mFormat = fmt;
            }
            if (mCodecCtx->channel_layout == AV_CH_LAYOUT_MONO) {
                mDstChanLayout = mCodecCtx->channel_layout;
                mFrameSize *= 1;
                mFormat = AL_FORMAT_MONO8;
            }
            if (mCodecCtx->channel_layout == 0 && mCodecCtx->channels >= 4 &&
                alIsExtensionPresent("AL_EXT_BFORMAT") &&
                (fmt = alGetEnumValue("AL_FORMAT_BFORMAT3D8")) != AL_NONE && fmt != -1) {
                int order{static_cast<int>(std::sqrt(mCodecCtx->channels)) - 1};
                if ((order + 1) * (order + 1) == mCodecCtx->channels ||
                    (order + 1) * (order + 1) + 2 == mCodecCtx->channels) {
                    mFrameSize *= 4;
                    mFormat = fmt;
                }
            }
            if (!mFormat) {
                mDstChanLayout = AV_CH_LAYOUT_STEREO;
                mFrameSize *= 2;
                mFormat = AL_FORMAT_STEREO8;
            }
        }
        if (!mFormat) {
            mDstSampleFmt = AV_SAMPLE_FMT_S16;
            mFrameSize = 2;
            if (mCodecCtx->channel_layout == AV_CH_LAYOUT_7POINT1 &&
                alIsExtensionPresent("AL_EXT_MCFORMATS") &&
                (fmt = alGetEnumValue("AL_FORMAT_71CHN16")) != AL_NONE && fmt != -1) {
                mDstChanLayout = mCodecCtx->channel_layout;
                mFrameSize *= 8;
                mFormat = fmt;
            }
            if ((mCodecCtx->channel_layout == AV_CH_LAYOUT_5POINT1 ||
                 mCodecCtx->channel_layout == AV_CH_LAYOUT_5POINT1_BACK) &&
                alIsExtensionPresent("AL_EXT_MCFORMATS") &&
                (fmt = alGetEnumValue("AL_FORMAT_51CHN16")) != AL_NONE && fmt != -1) {
                mDstChanLayout = mCodecCtx->channel_layout;
                mFrameSize *= 6;
                mFormat = fmt;
            }
            if (mCodecCtx->channel_layout == AV_CH_LAYOUT_MONO) {
                mDstChanLayout = mCodecCtx->channel_layout;
                mFrameSize *= 1;
                mFormat = AL_FORMAT_MONO16;
            }
            if (mCodecCtx->channel_layout == 0 && mCodecCtx->channels >= 4 &&
                alIsExtensionPresent("AL_EXT_BFORMAT") &&
                (fmt = alGetEnumValue("AL_FORMAT_BFORMAT3D16")) != AL_NONE && fmt != -1) {
                int order{static_cast<int>(std::sqrt(mCodecCtx->channels)) - 1};
                if ((order + 1) * (order + 1) == mCodecCtx->channels ||
                    (order + 1) * (order + 1) + 2 == mCodecCtx->channels) {
                    mFrameSize *= 4;
                    mFormat = fmt;
                }
            }
            if (!mFormat) {
                mDstChanLayout = AV_CH_LAYOUT_STEREO;
                mFrameSize *= 2;
                mFormat = AL_FORMAT_STEREO16;
            }
        }
        void *samples{nullptr};
        ALsizei buffer_len = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(
                mCodecCtx->sample_rate * AudioBufferTime).count() * mFrameSize);

        mSamples = nullptr;
        mSamplesMax = 0;
        mSamplesPos = 0;
        mSamplesLen = 0;

        mDecodedFrame = av_frame_alloc();
        if (!mDecodedFrame) {
            std::cerr << "Failed to allocate audio frame" << std::endl;
            av_freep(&samples);
            return 0;
        }

        if (!mDstChanLayout) {
            // OpenAL only supports first-order ambisonics with AL_EXT_BFORMAT, so
//             * we have to drop any extra channels.
            mSwresCtx = swr_alloc_set_opts(nullptr,
                                           ((int64_t) 1 << 4) - 1, mDstSampleFmt, mCodecCtx->sample_rate,
                                           ((int64_t) 1 << mCodecCtx->channels) - 1, mCodecCtx->sample_fmt, mCodecCtx->sample_rate,
                                           0, nullptr);

            // Note that ffmpeg/libavcodec has no method to check the ambisonic
//             * channel order and normalization, so we can only assume AmbiX as the
//             * defacto-standard. This is not true for .amb files, which use FuMa.

            std::vector<double> mtx(64 * 64, 0.0);
#ifdef AL_SOFT_bformat_ex
            ambi_layout = AL_ACN_SOFT;
            ambi_scale = AL_SN3D_SOFT;
            if (has_bfmt_ex) {
                // An identity matrix that doesn't remix any channels.
                std::cout << "Found AL_SOFT_bformat_ex" << std::endl;
                mtx[0 + 0 * 64] = 1.0;
                mtx[1 + 1 * 64] = 1.0;
                mtx[2 + 2 * 64] = 1.0;
                mtx[3 + 3 * 64] = 1.0;
            } else
#endif
            {
                std::cout << "Found AL_EXT_BFORMAT" << std::endl;
                // Without AL_SOFT_bformat_ex, OpenAL only supports FuMa channel
//                 * ordering and normalization, so a custom matrix is needed to
//                 * scale and reorder the source from AmbiX.

                mtx[0 + 0 * 64] = std::sqrt(0.5);
                mtx[3 + 1 * 64] = 1.0;
                mtx[1 + 2 * 64] = 1.0;
                mtx[2 + 3 * 64] = 1.0;
            }
            swr_set_matrix(mSwresCtx, mtx.data(), 64);
        } else
            mSwresCtx = swr_alloc_set_opts(nullptr,
                                           static_cast<int64_t>(mDstChanLayout), mDstSampleFmt, mCodecCtx->sample_rate,
                                           mCodecCtx->channel_layout ? static_cast<int64_t>(mCodecCtx->channel_layout) :
                                           av_get_default_channel_layout(mCodecCtx->channels),
                                           mCodecCtx->sample_fmt, mCodecCtx->sample_rate,
                                           0, nullptr);
        if (!mSwresCtx || swr_init(mSwresCtx) != 0) {
            std::cerr << "Failed to initialize audio converter" << std::endl;
            av_freep(&samples);
            return 0;
        }

        mBuffers.assign(AudioBufferTotalTime / AudioBufferTime, 0);
        alGenBuffers(static_cast<ALsizei>(mBuffers.size()), mBuffers.data());
        alGenSources(1, &mSource);

        if (alGetError() != AL_NO_ERROR) {
            av_freep(&samples);
            return 0;
        }

#ifdef AL_SOFT_bformat_ex
        if (has_bfmt_ex) {
            for (ALuint bufid : mBuffers) {
                alBufferi(bufid, AL_AMBISONIC_LAYOUT_SOFT, ambi_layout);
                alBufferi(bufid, AL_AMBISONIC_SCALING_SOFT, ambi_scale);
            }
        }
#endif
        samples = av_malloc(static_cast<ALuint>(buffer_len));

        // Prefill the codec buffer.
        do {
            const int ret{mPackets.sendTo(mCodecCtx)};
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                break;
        } while (1);

        srclock.lock();
        if (AudioSystem::alcGetInteger64vSOFT) {
            int64_t devtime{};
            AudioSystem::alcGetInteger64vSOFT(alcGetContextsDevice(alcGetCurrentContext()), ALC_DEVICE_CLOCK_SOFT,
                                              1, &devtime);
            mDeviceStartTime = std::chrono::nanoseconds{devtime} - mCurrentPts;
        }
        while (alGetError() == AL_NO_ERROR && !mMovie->mQuit &&
               mConnected) {
            mConnected = true;
            ALint processed, queued, state;

            // First remove any processed buffers.
            alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);
            while (processed > 0) {
                std::array<ALuint, 4> bids;
                const ALsizei todq{std::min<ALsizei>(bids.size(), processed)};
                alSourceUnqueueBuffers(mSource, todq, bids.data());
                processed -= todq;
            }

            // Refill the buffer queue.
            int sync_skip{getSync()};
            alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);
            while (static_cast<ALuint>(queued) < mBuffers.size()) {
                const ALuint bufid{mBuffers[mBufferIdx]};
                // Read the next chunk of data, filling the buffer, and queue it on
//                 * the source.

                {
                    auto ptr = static_cast<uint8_t *>(samples);
                    if (!readAudio(ptr, static_cast<unsigned int>(buffer_len), &sync_skip))
                        break;
                    alBufferData(bufid, mFormat, samples, buffer_len, mCodecCtx->sample_rate);
                }

                alSourceQueueBuffers(mSource, 1, &bufid);
                mBufferIdx = (mBufferIdx + 1) % mBuffers.size();
                ++queued;
            }
            if (queued == 0)
                break;

            // Check that the source is playing.
            alGetSourcei(mSource, AL_SOURCE_STATE, &state);
            if (state == AL_STOPPED) {
                // AL_STOPPED means there was an underrun. Clear the buffer queue
//                 * since this likely means we're late, and rewind the source to get
//                 * it back into an AL_INITIAL state.

                alSourceRewind(mSource);
                alSourcei(mSource, AL_BUFFER, 0);
                if (AudioSystem::alcGetInteger64vSOFT) {
                    // Also update the device start time with the current device
//                     * clock, so the decoder knows we're running behind.

                    int64_t devtime{};
                    AudioSystem::alcGetInteger64vSOFT(alcGetContextsDevice(alcGetCurrentContext()),
                                                      ALC_DEVICE_CLOCK_SOFT, 1, &devtime);
                    mDeviceStartTime = std::chrono::nanoseconds{devtime} - mCurrentPts;
                }
                continue;
            }

            // (re)start the source if needed, and wait for a buffer to finish
            if (state != AL_PLAYING && state != AL_PAUSED)
                startPlayback();

            mSrcCond.wait_for(srclock, sleep_time);
        }

        alSourceRewind(mSource);
        alSourcei(mSource, AL_BUFFER, 0);
        srclock.unlock();
        av_freep(&samples);
        return 0;
    }
}