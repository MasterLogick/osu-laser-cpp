//
// Created by MasterLogick on 1/23/20.
//
#ifndef OSU_LASER_C_OSU_H
#define OSU_LASER_C_OSU_H

#include "../interaction/Properties.h"
#include "../graphics/Graphics.h"
#include "Logic.h"
#include "../interaction/audio/AudioSystem.h"

namespace osu {
    extern bool shouldClose;

    class Osu {
    public:
        static void initialise() {
            std::locale::global(std::locale("en_US.UTF-8"));
            Properties::initialise();
            AudioSystem::initialise();
            Graphics::initialise();
            Logic::initialise();
        }

        static void start() {
            Logic::start();
            Graphics::start();
#ifndef NDEBUG
            std::cout << "Join to drawingThread" << std::endl;
#endif
            Graphics::drawingThread->join();
        }
    };
}

#endif