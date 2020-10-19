//
// Created by MasterLogick on 28/06/2020.
//

#ifndef OSU_LASER_CPP_EVBACKGROUND_H
#define OSU_LASER_CPP_EVBACKGROUND_H

#include "Event.h"
#include "DrawableEvent.h"
#include <vector>

namespace osu {
    class EvBackground : public DrawableEvent {
    public:
        std::string path;
        int xoff;
        int yoff;

        explicit EvBackground(std::vector<std::string> &data);
    };
}

#endif //OSU_LASER_CPP_EVBACKGROUND_H
