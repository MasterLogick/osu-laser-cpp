//
// Created by MasterLogick on 10/14/20.
//

#ifndef OSU_LASER_CPP_EVCOLOUR_H
#define OSU_LASER_CPP_EVCOLOUR_H

#include "Event.h"
#include <vector>

namespace osu {
    class EvColour : public Event {
        int r;
        int g;
        int b;
    public:
        explicit EvColour(std::vector<std::string> &data);
    };
}

#endif //OSU_LASER_CPP_EVCOLOUR_H
