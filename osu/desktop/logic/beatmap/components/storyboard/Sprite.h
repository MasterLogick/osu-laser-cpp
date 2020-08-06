//
// Created by MasterLogick on 6/9/20.
//

#ifndef OSU_LASER_CPP_SPRITE_H
#define OSU_LASER_CPP_SPRITE_H

#include "Layer.h"
#include "../../../../graphics/UI/Origin.h"
#include "../../../utill/Point.h"
#include "Event.h"
#include <string>
#include <vector>

namespace osu {
    class Sprite : public Event {
    public:
        Layer layer;
        Origin origin;
        std::string file;
        PointF pos;

        explicit Sprite(std::vector<std::string> &data);
    };
}


#endif //OSU_LASER_CPP_SPRITE_H
