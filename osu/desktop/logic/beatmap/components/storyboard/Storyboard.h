//
// Created by MasterLogick on 6/18/20.
//

#ifndef OSU_LASER_CPP_STORYBOARD_H
#define OSU_LASER_CPP_STORYBOARD_H

#include <vector>
#include "commands/Command.h"
#include "Sprite.h"
#include <list>

namespace osu {
    class Storyboard {
    private:
        std::vector<std::pair<int, std::list<Command *>>> timeBorders;
        std::vector<Sprite> sprites;
    };
}

#endif //OSU_LASER_CPP_STORYBOARD_H
