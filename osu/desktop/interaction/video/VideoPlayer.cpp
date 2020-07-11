//
// Created by MasterLogick on 7/10/20.
//

#include "VideoPlayer.h"

namespace osu{

    VideoPlayer::VideoPlayer(std::string filePath) : movState() {
        /* Register all formats and codecs */
//#if !(LIBAVFORMAT_VERSION_INT >= AV_VERSION_INT(58, 9, 100))
        av_register_all();
//#endif
        /* Initialize networking protocols */
        avformat_network_init();

        /* Open an audio device */
        ++argv; --argc;
        if(InitAL(&argv, &argc))
        {
            std::cerr<< "Failed to set up audio device" <<std::endl;
            return 1;
        }

        {
            auto device = alcGetContextsDevice(alcGetCurrentContext());
            if(alcIsExtensionPresent(device, "ALC_SOFT_device_clock"))
            {
                std::cout<< "Found ALC_SOFT_device_clock" <<std::endl;
                alcGetInteger64vSOFT = reinterpret_cast<LPALCGETINTEGER64VSOFT>(
                        alcGetProcAddress(device, "alcGetInteger64vSOFT")
                );
            }
        }

        if(alIsExtensionPresent("AL_SOFT_source_latency"))
        {
            std::cout<< "Found AL_SOFT_source_latency" <<std::endl;
            alGetSourcei64vSOFT = reinterpret_cast<LPALGETSOURCEI64VSOFT>(
                    alGetProcAddress("alGetSourcei64vSOFT")
            );
        }

        int fileidx{0};
        for(;fileidx < argc;++fileidx)
        {
            if(strcmp(argv[fileidx], "-direct") == 0)
            {
                if(alIsExtensionPresent("AL_SOFT_direct_channels_remix"))
                {
                    std::cout<< "Found AL_SOFT_direct_channels_remix" <<std::endl;
                    DirectOutMode = AL_REMIX_UNMATCHED_SOFT;
                }
                else if(alIsExtensionPresent("AL_SOFT_direct_channels"))
                {
                    std::cout<< "Found AL_SOFT_direct_channels" <<std::endl;
                    DirectOutMode = AL_DROP_UNMATCHED_SOFT;
                }
                else
                    std::cerr<< "AL_SOFT_direct_channels not supported for direct output" <<std::endl;
            }
            else if(strcmp(argv[fileidx], "-wide") == 0)
            {
                if(!alIsExtensionPresent("AL_EXT_STEREO_ANGLES"))
                    std::cerr<< "AL_EXT_STEREO_ANGLES not supported for wide stereo" <<std::endl;
                else
                {
                    std::cout<< "Found AL_EXT_STEREO_ANGLES" <<std::endl;
                    EnableWideStereo = true;
                }
            }
            else if(strcmp(argv[fileidx], "-novideo") == 0)
                DisableVideo = true;
            else
                break;
        }

        while(fileidx < argc && !movState)
        {
            movState = std::unique_ptr<MovieState>{new MovieState{argv[fileidx++]}};
            if(!movState->prepare()) movState = nullptr;
        }
        if(!movState)
        {
            std::cerr<< "Could not start a video" <<std::endl;
            return 1;
        }
        movState->setTitle(screen);

        /* Default to going to the next movie at the end of one. */
        enum class EomAction {
            Next, Quit
        } eom_action{EomAction::Next};
        seconds last_time{seconds::min()};
        while(1)
        {
            SDL_Event event{};
            int have_evt{SDL_WaitEventTimeout(&event, 10)};

            auto cur_time = std::chrono::duration_cast<seconds>(movState->getMasterClock());
            if(cur_time != last_time)
            {
                auto end_time = std::chrono::duration_cast<seconds>(movState->getDuration());
                std::cout<< "    \r "<<PrettyTime{cur_time}<<" / "<<PrettyTime{end_time} <<std::flush;
                last_time = cur_time;
            }

            bool force_redraw{false};
            if(have_evt) do {
                    switch(event.type)
                    {
                        case SDL_KEYDOWN:
                            switch(event.key.keysym.sym)
                            {
                                case SDLK_ESCAPE:
                                    movState->mQuit = true;
                                    eom_action = EomAction::Quit;
                                    break;

                                case SDLK_n:
                                    movState->mQuit = true;
                                    eom_action = EomAction::Next;
                                    break;

                                default:
                                    break;
                            }
                            break;

                        case SDL_WINDOWEVENT:
                            switch(event.window.event)
                            {
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
                            movState->mQuit = true;
                            eom_action = EomAction::Quit;
                            break;

                        case FF_MOVIE_DONE_EVENT:
                            std::cout<<'\n';
                            last_time = seconds::min();
                            if(eom_action != EomAction::Quit)
                            {
                                movState = nullptr;
                                while(fileidx < argc && !movState)
                                {
                                    movState = std::unique_ptr<MovieState>{new MovieState{argv[fileidx++]}};
                                    if(!movState->prepare()) movState = nullptr;
                                }
                                if(movState)
                                {
                                    movState->setTitle(screen);
                                    break;
                                }
                            }

                            /* Nothing more to play. Shut everything down and quit. */
                            movState = nullptr;

                            CloseAL();

                            SDL_DestroyRenderer(renderer);
                            renderer = nullptr;
                            SDL_DestroyWindow(screen);
                            screen = nullptr;

                            SDL_Quit();
                            exit(0);

                        default:
                            break;
                    }
                } while(SDL_PollEvent(&event));

            movState->mVideo.updateVideo(screen, renderer, force_redraw);
        }

        std::cerr<< "SDL_WaitEvent error - "<<SDL_GetError() <<std::endl;
        return 1;
    }
}