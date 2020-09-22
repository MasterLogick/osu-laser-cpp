//
// Created by MasterLogick on 6/9/20.
//

#ifndef OSU_LASER_CPP_ANIMATION_H
#define OSU_LASER_CPP_ANIMATION_H

#include "Layer.h"
#include "../../../../graphics/UI/Origin.h"
#include "../../../utill/Point.h"
#include "Event.h"
#include "DrawableEvent.h"
#include <string>
#include <vector>

namespace osu {
    class Animation : public DrawableEvent {
    public:
        Layer layer;
        Origin origin;
        std::string path;
        Point pos;
        int frameCount;
        float frameDelay;
        enum LoopType {
            LoopOnce, LoopForever
        } type;

        explicit Animation(std::vector<std::string> &data);
    };
}

#endif //OSU_LASER_CPP_ANIMATION_H
