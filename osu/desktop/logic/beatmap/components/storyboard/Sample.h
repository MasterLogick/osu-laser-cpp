//
// Created by MasterLogick on 6/9/20.
//

#ifndef OSU_LASER_CPP_SAMPLE_H
#define OSU_LASER_CPP_SAMPLE_H

#include "Layer.h"
#include <string>

namespace osu {
    class Sample {
    public:
        int time;
        Layer layer;
        std::string file;
        int volume;

        explicit Sample(std::string &line);
    };
}


#endif //OSU_LASER_CPP_SAMPLE_H
