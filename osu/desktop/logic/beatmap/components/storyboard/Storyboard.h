//
// Created by MasterLogick on 9/5/20.
//

#ifndef OSU_LASER_CPP_STORYBOARD_H
#define OSU_LASER_CPP_STORYBOARD_H

#include <vector>
#include "Event.h"
#include "PreCachedImage.h"
#include "commands/CompoundCommand.h"

namespace osu {
    class Storyboard {
    public:
        std::vector<CompoundCommand *> commandContainers;
        std::vector<Event *> events;
        std::vector<PreCachedImage> texturePaths;

        void cacheSprites();
    };
}

#endif //OSU_LASER_CPP_STORYBOARD_H
