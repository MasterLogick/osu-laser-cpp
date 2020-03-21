//
// Created by Masterlogick on 2/25/20.
//
#include "MediaFile.h"
#include <condition_variable>
#include <functional>
#include <algorithm>
#include <iostream>
#include <utility>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <array>
#include <cmath>
#include <deque>
#include <mutex>
#include <ratio>
#include <glad/glad.h>
#include <SDL_events.h>
#include <SDL.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavformat/version.h"
#include "libavutil/avutil.h"
#include "libavutil/error.h"
#include "libavutil/frame.h"
#include "libavutil/mem.h"
#include "libavutil/pixfmt.h"
#include "libavutil/rational.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/version.h"
#include "libavutil/channel_layout.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"

struct SwsContext;
struct SwrContext;
}


#include "alc.h"
#include "al.h"
#include "alext.h"

#include "../audio/common/alhelpers.h"
#include "../audio/AudioSystem.h"
#include "MovieState.h"

namespace osu {

    inline constexpr int64_t operator "" _i64(unsigned long long int n) noexcept { return static_cast<int64_t>(n); }

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

    static std::chrono::microseconds get_avtime() { return std::chrono::microseconds{av_gettime()}; }

// Define unique_ptrs to auto-cleanup associated ffmpeg objects
    struct AVIOContextDeleter {
        void operator()(AVIOContext *ptr) { avio_closep(&ptr); }
    };

    using AVIOContextPtr = std::unique_ptr<AVIOContext, AVIOContextDeleter>;

    struct AVFormatCtxDeleter {
        void operator()(AVFormatContext *ptr) { avformat_close_input(&ptr); }
    };

    using AVFormatCtxPtr = std::unique_ptr<AVFormatContext, AVFormatCtxDeleter>;

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

    struct SwsContextDeleter {
        void operator()(SwsContext *ptr) { sws_freeContext(ptr); }
    };

    using SwsContextPtr = std::unique_ptr<SwsContext, SwsContextDeleter>;


    MediaFile::MediaFile(char *name) : movState(name) {
        MovieState *ms = &movState;
        thr = new std::thread([ms, name]() {
//        std::unique_ptr<MovieState> movState;
            // Register all formats and codecs
#if !(LIBAVFORMAT_VERSION_INT >= AV_VERSION_INT(58, 9, 100))
            av_register_all();
#endif
            // Initialize networking protocols
            avformat_network_init();
            ms->prepare();
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
                std::cerr << "Could not initialize SDL - <<" << SDL_GetError() << std::endl;
                return;
            }
            SDL_Window *screen{SDL_CreateWindow("test", 0, 0, 640, 480, SDL_WINDOW_RESIZABLE)};
            Uint32 render_flags{SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};
            SDL_Renderer *renderer{SDL_CreateRenderer(screen, -1, render_flags)};
            if (renderer) {
                SDL_RendererInfo rinf{};
                bool ok{false};

                /* Make sure the renderer supports IYUV textures. If not, fallback to a
                 * software renderer. */
                if (SDL_GetRendererInfo(renderer, &rinf) == 0) {
                    for (Uint32 i{0u}; !ok && i < rinf.num_texture_formats; i++)
                        ok = (rinf.texture_formats[i] == SDL_PIXELFORMAT_IYUV);
                }
                if (!ok) {
                    std::cerr << "IYUV pixelformat textures not supported on renderer " << rinf.name << std::endl;
                    SDL_DestroyRenderer(renderer);
                    renderer = nullptr;
                }
            }
            if (!renderer) {
                render_flags = SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC;
                renderer = SDL_CreateRenderer(screen, -1, render_flags);
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, nullptr);
            SDL_RenderPresent(renderer);
//        if (!movState.prepare()){ movState = nullptr;}
/*
        if (movState) {
            std::cerr << "Could not start a video" << std::endl;
            return;
        }*/
            while (1) {
                SDL_Event event{};
                int have_evt{SDL_WaitEventTimeout(&event, 10)};

                bool force_redraw{false};
                if (have_evt)
                    do {
                        switch (event.type) {
                            case SDL_KEYDOWN:
                                switch (event.key.keysym.sym) {
                                    case SDLK_ESCAPE:
                                        ms->mQuit = true;
                                        break;

                                    case SDLK_n:
                                        ms->mQuit = true;
                                        break;

                                    default:
                                        break;
                                }
                                break;

                            case SDL_WINDOWEVENT:
                                switch (event.window.event) {
                                    case SDL_WINDOWEVENT_RESIZED:
                                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                        SDL_RenderFillRect(renderer, nullptr);
                                        force_redraw = true;
                                        break;

                                    case SDL_WINDOWEVENT_EXPOSED:
                                        force_redraw = true;
                                        break;

                                    default:
                                        break;
                                }
                                break;

                            case SDL_QUIT:
                                ms->mQuit = true;
                                break;

                            case SDL_USEREVENT:
                                std::cout << '\n';

                                /* Nothing more to play. Shut everything down and quit. */
//                            movState = nullptr;

                                CloseAL();

                                SDL_DestroyRenderer(renderer);
                                renderer = nullptr;
                                SDL_DestroyWindow(screen);
                                screen = nullptr;
                                return;
//                            SDL_Quit();

//                            exit(0);

                            default:
                                break;
                        }
                    } while (SDL_PollEvent(&event));
                ms->mVideo->updateVideo(screen, renderer, force_redraw);
            }
        });
    }

    MediaFile::~MediaFile() {
        thr->join();
        delete thr;
    }

    void MediaFile::draw(int x, int y) {
        movState.mVideo->draw(x, y);
    }
}
