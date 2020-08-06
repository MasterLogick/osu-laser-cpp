//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_VECTORSCALE_H
#define OSU_LASER_CPP_VECTORSCALE_H

#include "SimpleCommand.h"
#include "../../../../utill/Point.h"

namespace osu {
    class VectorScale : public SimpleCommand {
    public:
        PointF start;
        PointF end;

        explicit VectorScale(std::vector<std::string> &s);

        void process(int time);
    };
}

#endif //OSU_LASER_CPP_VECTORSCALE_H
