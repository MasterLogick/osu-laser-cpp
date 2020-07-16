//
// Created by MasterLogick on 7/10/20.
//

#include "alext.h"
#include "VideoPlayer.h"
#include "../audio/AudioSystem.h"

#include <memory>
#include <iostream>

namespace osu {

    //struct AVCodecCtxDeleter;
    //
    //    struct SwsContextDeleter;
    //
    //    struct AVFrameDeleter;
    //
    //    struct SwrContextDeleter;
    //
    //    struct AVIOContextDeleter;
    //
    //    struct AVFormatCtxDeleter;
    //
    //    typedef std::unique_ptr<AVCodecContext, AVCodecCtxDeleter> AVCodecCtxPtr;
    //
    //    typedef std::unique_ptr<SwsContext, SwsContextDeleter> SwsContextPtr;
    //
    //    typedef std::unique_ptr<AVFrame, AVFrameDeleter> AVFramePtr;
    //
    //    typedef std::unique_ptr<SwrContext, SwrContextDeleter> SwrContextPtr;
    //
    //    typedef std::unique_ptr<AVIOContext, AVIOContextDeleter> AVIOContextPtr;
    //
    //    typedef std::unique_ptr<AVFormatContext, AVFormatCtxDeleter> AVFormatCtxPtr;


    VideoPlayer::VideoPlayer(std::string filePath) : movState(), filePath(filePath) {

    }

    void VideoPlayer::initialise() {
/* Register all formats and codecs */
#if !(LIBAVFORMAT_VERSION_INT >= AV_VERSION_INT(58, 9, 100))
        av_register_all();
#endif
        av_log_set_level(AV_LOG_ERROR);
        movState = std::make_unique<MovieState>(filePath, SyncMaster::Default);
        if (!movState->prepare()) {
            std::cerr << "Could not start a video" << std::endl;
            return;
        }
        movState->mVideo.initialise();
        mainLoop = std::thread([this] {
            movState->start();
            /* Default to going to the next movie at the end of one. */
//        enum class EomAction {
//            Next, Quit
//        } eom_action{EomAction::Next};
//        seconds last_time{seconds::min()};
            while (!movState->mQuit) {
//            SDL_Event event{};
//            int have_evt{SDL_WaitEventTimeout(&event, 10)};
//
//            auto cur_time = std::chrono::duration_cast<seconds>(movState->getMasterClock());
//            if (cur_time != last_time) {
//                auto end_time = std::chrono::duration_cast<seconds>(movState->getDuration());
//                std::cout << "    \r " << PrettyTime{cur_time} << " / " << PrettyTime{end_time} << std::flush;
//                last_time = cur_time;
//            }
//
//            bool force_redraw{false};
//            if (have_evt)
//                do {
//                    switch (event.type) {
//                        case SDL_KEYDOWN: todo use in stop() method
//                            switch (event.key.keysym.sym) {
//                                case SDLK_ESCAPE:
//                                    movState->mQuit = true;
//                                    eom_action = EomAction::Quit;
//                                    break;
//
//                                case SDLK_n:
//                                    movState->mQuit = true;
//                                    eom_action = EomAction::Next;
//                                    break;
//
//                                default:
//                                    break;
//                            }
//                            break;
//
//                        case SDL_WINDOWEVENT:
//                            switch (event.window.event) {
//                                case SDL_WINDOWEVENT_RESIZED:
//                                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//                                    SDL_RenderFillRect(renderer, nullptr);
//                                    force_redraw = true;
//                                    break;
//
//                                case SDL_WINDOWEVENT_EXPOSED:
//                                    force_redraw = true;
//                                    break;
//
//                                default:
//                                    break;
//                            }
//                            break;
//
//                        case SDL_QUIT:
//                            movState->mQuit = true;
//                            eom_action = EomAction::Quit;
//                            break;
//
//                        case FF_MOVIE_DONE_EVENT:
//                            std::cout << '\n';
//                            last_time = seconds::min();
//                            if (eom_action != EomAction::Quit) {
//                                movState = nullptr;
//                                while (fileidx < argc && !movState) {
//                                    movState = std::unique_ptr<MovieState>{new MovieState{argv[fileidx++]}};
//                                    if (!movState->prepare()) movState = nullptr;
//                                }
//                                if (movState) {
//                                    movState->setTitle(screen);
//                                    break;
//                                }
//                            }
//
//                            /* Nothing more to play. Shut everything down and quit. */
//                            movState = nullptr;
//
//                            CloseAL();
//
//                            SDL_DestroyRenderer(renderer);
//                            renderer = nullptr;
//                            SDL_DestroyWindow(screen);
//                            screen = nullptr;
//
//                            SDL_Quit();
//                            exit(0);
//
//                        default:
//                            break;
//                    }
//                } while (SDL_PollEvent(&event));

                movState->mVideo.updateVideo();
            }
        });

//        std::cerr << "SDL_WaitEvent error - " << SDL_GetError() << std::endl;
//        return;
    }

    void VideoPlayer::stop() {

    }

    void VideoPlayer::pause() {

    }

    void VideoPlayer::setDirectOutMode(bool flag) {
        if (alIsExtensionPresent("AL_SOFT_direct_channels_remix")) {
//            std::cout << "Found AL_SOFT_direct_channels_remix" << std::endl;
            movState->mAudio.DirectOutMode = AL_REMIX_UNMATCHED_SOFT;
        } else if (alIsExtensionPresent("AL_SOFT_direct_channels")) {
//            std::cout << "Found AL_SOFT_direct_channels" << std::endl;
            movState->mAudio.DirectOutMode = AL_DROP_UNMATCHED_SOFT;
        } else {
//            std::cerr << "AL_SOFT_direct_channels not supported for direct output" << std::endl;
        }
    }

    void VideoPlayer::setWideStereo(bool flag) {
        if (!alIsExtensionPresent("AL_EXT_STEREO_ANGLES")) {
//            std::cerr << "AL_EXT_STEREO_ANGLES not supported for wide stereo" << std::endl;
        } else {
//            std::cout << "Found AL_EXT_STEREO_ANGLES" << std::endl;
            movState->mAudio.EnableWideStereo = true;
        }
    }

    void VideoPlayer::draw(int x, int y) {
        movState->mVideo.draw(x, y);
    }
}