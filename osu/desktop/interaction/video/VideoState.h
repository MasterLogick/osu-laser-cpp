//
// Created by MasterLogick on 3/8/20.
//

#ifndef OSU_LASER_C_VIDEOSTATE_H
#define OSU_LASER_C_VIDEOSTATE_H

#include <chrono>
#include <mutex>
#include <condition_variable>

#include "glad/glad.h"

extern "C" {
#include  <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
struct SwsContext;
}

#include "MovieState.h"
#include "PacketQueue.h"

#define PBO_AMOUNT 3
namespace osu {
    class MovieState;

    class VideoState {
        MovieState *mMovie;
        // The pts of the currently displayed frame, and the time (av_gettime) it was last updated - used to have running video pts

        std::chrono::nanoseconds mDisplayPts{0};

        std::chrono::microseconds mDisplayPtsTime{std::chrono::microseconds::min()};
        std::mutex mDispPtsMutex;
        // Swscale context for format conversion

        SwsContext *mSwscaleCtx;

        struct Picture {
            AVFrame *mFrame{};
            std::chrono::nanoseconds mPts{std::chrono::nanoseconds::min()};
        };

        std::array<Picture, 24> mPictQ;
        size_t mPictQRead{0u}, mPictQWrite{1u};
        GLuint yPlaneTexture;
        GLuint uPlaneTexture;
        GLuint vPlaneTexture;
        GLuint unpackPBO[PBO_AMOUNT];
        uint8_t *mappedPBO[PBO_AMOUNT];
        GLuint drawVAO;
        GLuint drawVBO;
        bool mFirstUpdate{true};
        std::mutex pboLock;

    public:
        bool redraw;

        bool mFinalUpdate{false};

        bool mEOS{false};

        std::condition_variable mPictQCond;
        std::mutex mPictQMutex;

        AVCodecContext *mCodecCtx;
        PacketQueue mPackets{14 * 1024 * 1024};

        void initialise();

        void draw(float x, float y);

        std::chrono::nanoseconds getClock();

        void updateVideo();

        explicit VideoState(MovieState *movie) : mMovie(movie) {}

        ~VideoState();

        int handler();

        AVStream *mStream{nullptr};
    };
}


#endif //OSU_LASER_C_VIDEOSTATE_H
