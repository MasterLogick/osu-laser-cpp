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
    struct SBTimePoint {
        int time;
        CompoundCommand *ptr;
    };

    class Storyboard {
    private:
        std::vector<SBTimePoint> timePoints;
        std::vector<Event *> events;
    public:

        void addEvent(Event *event);

        void addEventCommandContainer(CompoundCommand *container);
    };
}

#endif //OSU_LASER_CPP_STORYBOARD_H
