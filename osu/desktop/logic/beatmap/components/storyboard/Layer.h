//
// Created by MasterLogick on 6/7/20.
//

#ifndef OSU_LASER_CPP_LAYER_H
#define OSU_LASER_CPP_LAYER_H

#include "../../../utill/str_switch.h"

namespace osu {
    enum Layer {
        Background = 0,
        Fail = 1,
        Pass = 2,
        Foreground = 3
    };

    Layer parseLayer(std::string &s);
}

#endif //OSU_LASER_CPP_LAYER_H