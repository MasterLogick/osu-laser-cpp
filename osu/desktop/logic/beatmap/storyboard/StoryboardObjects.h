//
// Created by MasterLogick on 6/6/20.
//

#ifndef OSU_LASER_CPP_STORYBOARDOBJECTS_H
#define OSU_LASER_CPP_STORYBOARDOBJECTS_H

#include <string>
#include "../../utill/StringUtills.h"
#include <boost/lexical_cast.hpp>
#include "Layer.h"

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

#endif //OSU_LASER_CPP_STORYBOARDOBJECTS_H
