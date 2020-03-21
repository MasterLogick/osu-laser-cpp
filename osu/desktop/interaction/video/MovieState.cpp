//
// Created by Masterlogick on 3/8/20.
//
#include <functional>
#include <iostream>
#include "MovieState.h"

extern "C" {
#include <libavutil/time.h>
}

namespace osu {
    static std::chrono::microseconds get_avtime() { return std::chrono::microseconds{av_gettime()}; }

    MovieState::~MovieState() {
        mQuit = true;
        if (mParseThread.joinable())
            mParseThread.join();
    }

    int MovieState::decode_interrupt_cb(void *ctx) {
        return static_cast<MovieState *>(ctx)->mQuit;
    }

    bool MovieState::prepare() {
        AVIOInterruptCB intcb{decode_interrupt_cb, this};
        if (avio_open2(&avioctx, mFilename.c_str(), AVIO_FLAG_READ, &intcb, nullptr)) {
            std::cerr << "Failed to open " << mFilename << std::endl;
            return false;
        }

        // Open movie file. If avformat_open_input fails it will automatically free
//         * this context, so don't set it onto a smart pointer yet.

        fmtctx = avformat_alloc_context();
        fmtctx->pb = avioctx;
        fmtctx->interrupt_callback = intcb;
        if (avformat_open_input(&fmtctx, mFilename.c_str(), nullptr, nullptr) != 0) {
            std::cerr << "Failed to open " << mFilename << std::endl;
            return false;
        }
        // Retrieve stream information
        if (avformat_find_stream_info(fmtctx, nullptr) < 0) {
            std::cerr << mFilename << ": failed to find stream info" << std::endl;
            return false;
        }

        mParseThread = std::thread{std::mem_fn(&MovieState::parse_handler), this};
        return true;
    }

    std::chrono::nanoseconds MovieState::getClock() {
        if (mClockBase == std::chrono::microseconds::min())
            return std::chrono::nanoseconds::zero();
        return get_avtime() - mClockBase;
    }

    std::chrono::nanoseconds MovieState::getMasterClock() {
        if (hasAudio)
            return mAudio->getClock();
        return mVideo->getClock();
    }

    std::chrono::nanoseconds MovieState::getDuration() {
        return std::chrono::duration<int64_t, std::ratio<1, AV_TIME_BASE>>(fmtctx->duration);
    }

    int MovieState::streamComponentOpen(unsigned int stream_index) {
        if (stream_index >= fmtctx->nb_streams)
            return -1;

        // Get a pointer to the codec context for the stream, and open the
//         * associated codec.

        AVCodecContext *avctx{avcodec_alloc_context3(nullptr)};
        if (!avctx) return -1;

        if (avcodec_parameters_to_context(avctx, fmtctx->streams[stream_index]->codecpar))
            return -1;

        AVCodec *codec{avcodec_find_decoder(avctx->codec_id)};
        if (!codec || avcodec_open2(avctx, codec, nullptr) < 0) {
            std::cerr << "Unsupported codec: " << avcodec_get_name(avctx->codec_id)
                      << " (0x" << std::hex << avctx->codec_id << std::dec << ")" << std::endl;
            return -1;
        }

        // Initialize and start the media type handler
        switch (avctx->codec_type) {
            case AVMEDIA_TYPE_AUDIO:
                mAudio->mStream = fmtctx->streams[stream_index];
                mAudio->mCodecCtx = std::move(avctx);
                break;

            case AVMEDIA_TYPE_VIDEO:
                mVideo->mStream = fmtctx->streams[stream_index];
                mVideo->mCodecCtx = std::move(avctx);
                break;

            default:
                return -1;
        }

        return static_cast<int>(stream_index);
    }

    int MovieState::parse_handler() {
        auto &audio_queue = mAudio->mPackets;
        auto &video_queue = mVideo->mPackets;

        int video_index{-1};
        int audio_index{-1};
        // Find the first video and audio streams
        for (unsigned int i{0u}; i < fmtctx->nb_streams; i++) {
            auto codecpar = fmtctx->streams[i]->codecpar;
            if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO && !DisableVideo && video_index < 0)
                video_index = streamComponentOpen(i);
            else if (codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audio_index < 0)
                audio_index = streamComponentOpen(i);
        }

        if (video_index < 0 && audio_index < 0) {
            std::cerr << mFilename << ": could not open codecs" << std::endl;
            mQuit = true;
        }

        // Set the base time 750ms ahead of the current av time.
        mClockBase = get_avtime() + std::chrono::milliseconds{750};
        hasVideo = false;
        hasAudio = false;
        if (audio_index >= 0) {
            mAudioThread = std::thread{std::mem_fn(&AudioState::handler), mAudio};
            hasAudio = true;
        }
        if (video_index >= 0) {
            mVideoThread = std::thread{std::mem_fn(&VideoState::handler), mVideo};
            hasVideo = true;
        }

        // Main packet reading/dispatching loop
        while (!mQuit) {
            AVPacket packet;
            if (av_read_frame(fmtctx, &packet) < 0) {
                break;
            }
            // Copy the packet into the queue it's meant for.
            if (packet.stream_index == video_index) {
                while (!mQuit && !video_queue.put(&packet))
                    std::this_thread::sleep_for(std::chrono::milliseconds{100});
            } else if (packet.stream_index == audio_index) {
                while (!mQuit && !audio_queue.put(&packet))
                    std::this_thread::sleep_for(std::chrono::milliseconds{100});
            }

            av_packet_unref(&packet);
        }
        // Finish the queues so the receivers know nothing more is coming.
        if (mVideo->mCodecCtx) video_queue.setFinished();
        if (mAudio->mCodecCtx) audio_queue.setFinished();

        // all done - wait for it
        if (mVideoThread.joinable())
            mVideoThread.join();
        if (mAudioThread.joinable())
            mAudioThread.join();

        mVideo->mEOS = true;
        std::unique_lock<std::mutex> lock{mVideo->mPictQMutex};
        while (!mVideo->mFinalUpdate)
            mVideo->mPictQCond.wait(lock);
        lock.unlock();
        mQuit = true;

        //test
        SDL_Event evt{};
        evt.user.type = SDL_USEREVENT;
        SDL_PushEvent(&evt);
        //test

        return 0;
    }

    MovieState::MovieState(std::string fname) {
        mAudio = new AudioState(this);
        mVideo = new VideoState(this);
        mFilename = std::move(fname);
    }
}
