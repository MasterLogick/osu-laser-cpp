//
// Created by MasterLogick on 6/9/20.
//

#ifndef OSU_LASER_CPP_SPRITE_H
#define OSU_LASER_CPP_SPRITE_H

#include "Layer.h"
#include "../../../graphics/UI/Origin.h"
#include "../../utill/Point.h"
#include <string>

namespace osu {
    class Sprite {
    public:
        Layer layer;
        Origin origin;
        std::string file;
        Point pos;
        int lifetimeStart;
        int lifetimeEnd;
        //todo add events container

        explicit Sprite(std::string &line);
    };
}


#endif //OSU_LASER_CPP_SPRITE_H
