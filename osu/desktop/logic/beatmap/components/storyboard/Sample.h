//
// Created by MasterLogick on 6/9/20.
//

#ifndef OSU_LASER_CPP_SAMPLE_H
#define OSU_LASER_CPP_SAMPLE_H

#include "Layer.h"
#include "Event.h"
#include <string>
#include <vector>

namespace osu {
    class Sample : public Event {
    public:
        Layer layer;
        std::string file;
        int volume;

        explicit Sample(std::vector<std::string> &data);
    };
}


#endif //OSU_LASER_CPP_SAMPLE_H
