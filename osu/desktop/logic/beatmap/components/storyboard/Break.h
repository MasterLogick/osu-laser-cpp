//
// Created by MasterLogick on 28/06/2020.
//

#ifndef OSU_LASER_CPP_BREAK_H
#define OSU_LASER_CPP_BREAK_H

#include "Event.h"
#include <vector>

namespace osu {
    class Break : public Event {
    public:
        int startTime;
        int endTime;

        explicit Break(std::vector<std::string> &data);
    };
}

#endif //OSU_LASER_CPP_BREAK_H
