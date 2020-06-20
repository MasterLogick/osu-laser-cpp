//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_EVENTTYPE_H
#define OSU_LASER_CPP_EVENTTYPE_H

#include <string>

namespace osu {
    enum EventType {
        Background = 0,
        Video = 1,
        Break = 2,
        Colour = 3,
        Sprite = 4,
        Sample = 5,
        Animation = 6
    };

    EventType parseEventType(std::string &s);
}

#endif //OSU_LASER_CPP_EVENTTYPE_H
