//
// Created by MasterLogick on 7/8/20.
//

#ifndef OSU_LASER_CPP_AUDIOSTATE_H
#define OSU_LASER_CPP_AUDIOSTATE_H

#include <memory>
#include <vector>
#include <chrono>
#include <al.h>
#include "PacketQueue.h"
#include "MovieState.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}
namespace osu {
    struct AVCodecCtxDeleter {
        void operator()(AVCodecContext *ptr) { avcodec_free_context(&ptr); }
    };

    using AVCodecCtxPtr = std::unique_ptr<AVCodecContext, AVCodecCtxDeleter>;

    struct AVFrameDeleter {
        void operator()(AVFrame *ptr) { av_frame_free(&ptr); }
    };

    using AVFramePtr = std::unique_ptr<AVFrame, AVFrameDeleter>;

    struct SwrContextDeleter {
        void operator()(SwrContext *ptr) { swr_free(&ptr); }
    };

    using SwrContextPtr = std::unique_ptr<SwrContext, SwrContextDeleter>;

    class AudioState {
        MovieState &mMovie;

        AVStream *mStream{nullptr};
        AVCodecCtxPtr mCodecCtx;

        PacketQueue mPackets{2 * 1024 * 1024};

        /* Used for clock difference average computation */
        std::chrono::duration<double> mClockDiffAvg{0};

        /* Time of the next sample to be buffered */
        std::chrono::nanoseconds mCurrentPts{0};

        /* Device clock time that the stream started at. */
        std::chrono::nanoseconds mDeviceStartTime{std::chrono::nanoseconds::min()};

        /* Decompressed sample frame, and swresample context for conversion */
        AVFramePtr mDecodedFrame;
        SwrContextPtr mSwresCtx;

        /* Conversion format, for what gets fed to OpenAL */
        uint64_t mDstChanLayout{0};
        AVSampleFormat mDstSampleFmt{AV_SAMPLE_FMT_NONE};

        /* Storage of converted samples */
        uint8_t *mSamples{nullptr};
        int mSamplesLen{0}; /* In samples */
        int mSamplesPos{0};
        int mSamplesMax{0};

        /* OpenAL format */
        ALenum mFormat{AL_NONE};
        ALuint mFrameSize{0};

        std::mutex mSrcMutex;
        std::condition_variable mSrcCond;
        std::atomic_flag mConnected;
        ALuint mSource{0};
        std::vector<ALuint> mBuffers;
        ALuint mBufferIdx{0};

        explicit AudioState(MovieState &movie);

        ~AudioState();

        std::chrono::nanoseconds getClockNoLock();

        std::chrono::nanoseconds getClock();

        void startPlayback();

        int getSync();

        int decodeFrame();

        bool readAudio(uint8_t *samples, unsigned int length, int *sample_skip);

    public:
        int handler();
    };
}


#endif //OSU_LASER_CPP_AUDIOSTATE_H
