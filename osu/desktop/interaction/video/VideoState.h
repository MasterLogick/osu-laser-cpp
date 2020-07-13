//
// Created by MasterLogick on 7/9/20.
//

#ifndef OSU_LASER_CPP_VIDEOSTATE_H
#define OSU_LASER_CPP_VIDEOSTATE_H


#include <memory>
#include <atomic>
#include <chrono>
#include "PacketQueue.h"
#include "glad/glad.h"
#include "MovieState.h"
#include "UniquePtrs.h"

extern "C" {
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
struct SwsContext;
}
#define VIDEO_PICTURE_QUEUE_SIZE 24
#define PBO_AMOUNT 3
namespace osu {
    class MovieState;

    class VideoState {
        MovieState &mMovie;

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
        GLuint yPlaneTexture;
        GLuint uPlaneTexture;
        GLuint vPlaneTexture;
        GLuint unpackPBO[PBO_AMOUNT];
        uint8_t *mappedPBO[PBO_AMOUNT];
        GLuint drawVAO;
        GLuint drawVBO;
        std::mutex pboLock;
        bool mFirstUpdate{true};
        bool redraw{true};

    public:

        int handler();

        void draw(float x, float y);

        void initialise();

        explicit VideoState(MovieState &movie);

        ~VideoState();

        std::chrono::nanoseconds getClock();

        AVStream *mStream{nullptr};
        AVCodecCtxPtr mCodecCtx;
        PacketQueue mPackets{14 * 1024 * 1024};
        std::atomic<bool> mFinalUpdate{false};
        std::condition_variable mPictQCond;
        std::atomic<bool> mEOS{false};
        std::mutex mPictQMutex;

        void updateVideo();
    };
}


#endif //OSU_LASER_CPP_VIDEOSTATE_H
