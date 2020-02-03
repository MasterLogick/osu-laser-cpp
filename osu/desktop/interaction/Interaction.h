//
// Created by user on 1/24/20.
//

#ifndef OSU_LASER_C_INTERACTION_H
#define OSU_LASER_C_INTERACTION_H

#include "audio/AudioSystem.h"
#include "input/Input.h"

namespace osu {
    class Interaction {
    public:
        static void initialise() {
            AudioSystem::initialise();
            Input::initialise();
        }

        static void start() {

        }
    };
}

#endif //OSU_LASER_C_INTERACTION_H
