//
// Created by MasterLogick on 6/7/20.
//

#ifndef OSU_LASER_CPP_LAYER_H
#define OSU_LASER_CPP_LAYER_H

#include "../../../utill/str_switch.h"

namespace osu {
    enum Layer {
        LBackground = 0,
        LFail = 1,
        LPass = 2,
        LForeground = 3
    };

    Layer parseLayer(std::string &s);
}

#endif //OSU_LASER_CPP_LAYER_H