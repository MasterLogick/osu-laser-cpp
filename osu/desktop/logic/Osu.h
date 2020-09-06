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
        static void initialise();

        static void start();
    };
}

#endif