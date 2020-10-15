//
// Created by MasterLogick on 6/9/20.
//

#ifndef OSU_LASER_CPP_EVSAMPLE_H
#define OSU_LASER_CPP_EVSAMPLE_H

#include "Layer.h"
#include "Event.h"
#include <string>
#include <vector>

namespace osu {
    class EvSample : public Event {
    public:
        Layer layer;
        std::string file;
        int volume;

        explicit EvSample(std::vector<std::string> &data);
    };
}


#endif //OSU_LASER_CPP_EVSAMPLE_H
