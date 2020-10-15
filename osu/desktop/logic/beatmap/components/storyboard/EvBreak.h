//
// Created by MasterLogick on 28/06/2020.
//

#ifndef OSU_LASER_CPP_EVBREAK_H
#define OSU_LASER_CPP_EVBREAK_H

#include "Event.h"
#include <vector>

namespace osu {
    class EvBreak : public Event {
    public:
        int endTime;

        explicit EvBreak(std::vector<std::string> &data);
    };
}

#endif //OSU_LASER_CPP_EVBREAK_H
