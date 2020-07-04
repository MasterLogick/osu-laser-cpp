//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_MOVEX_H
#define OSU_LASER_CPP_MOVEX_H

#include "SimpleCommand.h"

namespace osu {
    class MoveX : public SimpleCommand {
    public:
        float startX;
        float endX;

        explicit MoveX(std::vector<std::string> &s);
    };
}

#endif //OSU_LASER_CPP_MOVEX_H
