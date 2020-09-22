//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_EVENTTYPE_H
#define OSU_LASER_CPP_EVENTTYPE_H

#include <string>

namespace osu {
    enum EventType {
        ETBackground = 0,
        ETVideo = 1,
        ETBreak = 2,
        ETColour = 3,
        ETSprite = 4,
        ETSample = 5,
        ETAnimation = 6
    };

    bool isDrawable(EventType e);

    EventType parseEventType(std::string &s);
}

#endif //OSU_LASER_CPP_EVENTTYPE_H
