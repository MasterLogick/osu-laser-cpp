//
// Created by MasterLogick on 7/9/20.
//

#ifndef OSU_LASER_CPP_VIDEOSTATE_H
#define OSU_LASER_CPP_VIDEOSTATE_H


#include <memory>
#include <atomic>
#include <chrono>

extern "C" {
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
struct SwsContext;
}

#include "PacketQueue.h"
#include "MovieState.h"

namespace osu {
    struct AVCodecCtxDeleter {
        void operator()(AVCodecContext *ptr) { avcodec_free_context(&ptr); }
    };

    using AVCodecCtxPtr = std::unique_ptr<AVCodecContext, AVCodecCtxDeleter>;

    struct SwsContextDeleter {
        void operator()(SwsContext *ptr) { sws_freeContext(ptr); }
    };

    using SwsContextPtr = std::unique_ptr<SwsContext, SwsContextDeleter>;

    struct AVFrameDeleter {
        void operator()(AVFrame *ptr) { av_frame_free(&ptr); }
    };

    using AVFramePtr = std::unique_ptr<AVFrame, AVFrameDeleter>;

    class VideoState {
        MovieState &mMovie;

        AVStream *mStream{nullptr};
        AVCodecCtxPtr mCodecCtx;

        PacketQueue mPackets{14 * 1024 * 1024};

        /* The pts of the currently displayed frame, and the time (av_gettime) it
         * was last updated - used to have running video pts
         */
        std::chrono::nanoseconds mDisplayPts{0};
        std::chrono::microseconds mDisplayPtsTime{std::chrono::microseconds::min()};
        std::mutex mDispPtsMutex;

        /* Swscale context for format conversion */
        SwsContextPtr mSwscaleCtx;

        struct Picture {
            AVFramePtr mFrame{};
            std::chrono::nanoseconds mPts{std::chrono::nanoseconds::min()};
        };
        std::array<Picture, VIDEO_PICTURE_QUEUE_SIZE> mPictQ;
        std::atomic<size_t> mPictQRead{0u}, mPictQWrite{1u};
        std::mutex mPictQMutex;
        std::condition_variable mPictQCond;

        SDL_Texture *mImage{nullptr};
        int mWidth{0}, mHeight{0}; /* Logical image size (actual size may be larger) */
        bool mFirstUpdate{true};

        std::atomic<bool> mEOS{false};
        std::atomic<bool> mFinalUpdate{false};

        VideoState(MovieState &movie);

        ~VideoState();

        std::chrono::nanoseconds getClock();

        void display(SDL_Window *screen, SDL_Renderer *renderer);

        void updateVideo(SDL_Window *screen, SDL_Renderer *renderer, bool redraw);

    public:
        int handler();
    };
}


#endif //OSU_LASER_CPP_VIDEOSTATE_H
