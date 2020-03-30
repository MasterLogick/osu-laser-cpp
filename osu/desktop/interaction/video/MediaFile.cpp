//
// Created by Masterlogick on 2/25/20.
//
#include "MediaFile.h"
#include <iostream>
#include <thread>
#include <cmath>
#include <deque>
#include <SDL_events.h>
#include <SDL.h>

namespace osu {

    inline constexpr int64_t operator "" _i64(unsigned long long int n) noexcept { return static_cast<int64_t>(n); }

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

    MediaFile::MediaFile(char *name) : movState(name), initLocker(initLock) {
        MediaFile *mf = this;
        thr = new std::thread([mf, name]() {
            if (!mf->initialised)
                mf->_lock.wait(mf->initLocker, [mf]() -> bool { return mf->initialised; });
            // Register all formats and codecs
#if !(LIBAVFORMAT_VERSION_INT >= AV_VERSION_INT(58, 9, 100))
            av_register_all();
#endif
            // Initialize networking protocols
            avformat_network_init();
            mf->movState.prepare();
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
            while (true) {
                SDL_Event event{};
                int have_evt{SDL_WaitEventTimeout(&event, 10)};

                bool force_redraw{false};
                if (have_evt)
                    do {
                        switch (event.type) {
                            case SDL_KEYDOWN:
                                switch (event.key.keysym.sym) {
                                    case SDLK_ESCAPE:
                                        mf->movState.mQuit = true;
                                        break;

                                    case SDLK_n:
                                        mf->movState.mQuit = true;
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
                                mf->movState.mQuit = true;
                                break;

                            case SDL_USEREVENT:
                                /* Nothing more to play. Shut everything down and quit. */
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(screen);
                                return;
                            default:
                                break;
                        }
                    } while (SDL_PollEvent(&event));
                mf->movState.mVideo->updateVideo(screen, renderer);
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

    void MediaFile::initialise() {
        movState.mVideo->initialise();
        initialised = true;
        _lock.notify_one();
    }
}
