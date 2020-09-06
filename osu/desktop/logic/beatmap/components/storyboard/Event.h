//
// Created by MasterLogick on 28/06/2020.
//

#ifndef OSU_LASER_CPP_EVENT_H
#define OSU_LASER_CPP_EVENT_H

#include "EventType.h"

namespace osu {
    class Event {
    public:
        EventType eventType;
        int startTime;
    };
}
#endif //OSU_LASER_CPP_EVENT_H
