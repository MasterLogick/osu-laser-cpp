//
// Created by MasterLogick on 6/18/20.
//

#ifndef OSU_LASER_CPP_STORYBOARD_H
#define OSU_LASER_CPP_STORYBOARD_H

#include "Sprite.h"
#include "Animation.h"
#include "Sample.h"
#include "Event.h"
#include "commands/CompoundCommand.h"
#include <list>
#include <vector>

namespace osu {
    class Storyboard {
    private:
        std::vector<CompoundCommand *> containers;
        std::vector<Event *> events;
        std::string dir;
    public:
        void addEvent(Event *event);

        void addEventCommandContainer(CompoundCommand *container);

        void cacheSprites();

        void setDirectory(std::string dir);
    };
}

#endif //OSU_LASER_CPP_STORYBOARD_H
