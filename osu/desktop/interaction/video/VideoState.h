//
// Created by Masterlogick on 3/8/20.
//

#ifndef OSU_LASER_C_VIDEOSTATE_H
#define OSU_LASER_C_VIDEOSTATE_H

#include <chrono>
#include <mutex>
#include <condition_variable>
#include <SDL.h>

#include "glad/glad.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
struct SwsContext;
}

#include "MovieState.h"
#include "PacketQueue.h"

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
//        GLuint mTexture;
        SDL_Texture *mImage{nullptr};
        int mWidth{0}, mHeight{0}; // Logical image size (actual size may be larger)
        bool mFirstUpdate{true};

        void display(SDL_Window *screen, SDL_Renderer *renderer);

    public:
        void draw(int x, int y);

        std::chrono::nanoseconds getClock();

        void updateVideo(SDL_Window *screen, SDL_Renderer *renderer, bool redraw);

        std::condition_variable mPictQCond;

        bool mFinalUpdate{false};

        std::mutex mPictQMutex;

        bool mEOS{false};

        explicit VideoState(MovieState *movie) : mMovie{movie} {}

        ~VideoState();

        int handler();

        AVStream *mStream{nullptr};

        AVCodecContext *mCodecCtx;

        PacketQueue mPackets{14 * 1024 * 1024};
    };
}


#endif //OSU_LASER_C_VIDEOSTATE_H
