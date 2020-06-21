//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_MOVEY_H
#define OSU_LASER_CPP_MOVEY_H

#include "SimpleCommand.h"

namespace osu {
    class MoveY : public SimpleCommand {
    public:
        float startY;
        float endY;

        explicit MoveY(std::vector<std::string> &s);
    };
}

#endif //OSU_LASER_CPP_MOVEY_H
