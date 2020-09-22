//
// Created by MasterLogick on 9/9/20.
//

#ifndef OSU_LASER_CPP_PRECACHEDIMAGE_H
#define OSU_LASER_CPP_PRECACHEDIMAGE_H

#include "Event.h"
#include "DrawableEvent.h"

namespace osu {
    struct PreCachedImage {
        std::vector<std::string> framesPaths;
        std::vector<DrawableEvent *> events;
    };
}
#endif //OSU_LASER_CPP_PRECACHEDIMAGE_H
