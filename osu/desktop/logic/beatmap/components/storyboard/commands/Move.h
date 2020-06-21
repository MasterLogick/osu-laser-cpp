//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_MOVE_H
#define OSU_LASER_CPP_MOVE_H

#include "SimpleCommand.h"
#include "../../../../utill/Point.h"

namespace osu {
    class Move : public SimpleCommand {
    public:
        PointF start;
        PointF end;

        explicit Move(std::vector<std::string> &s);
    };
}

#endif //OSU_LASER_CPP_MOVE_H
