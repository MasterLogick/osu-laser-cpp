//
// Created by MasterLogick on 2/5/20.
//

#include "Interaction.h"

namespace osu {

    void Interaction::initialise() {
        AudioSystem::initialise();
        Input::initialise();
    }

    void Interaction::start() {
        AudioSystem::start();

    }
}