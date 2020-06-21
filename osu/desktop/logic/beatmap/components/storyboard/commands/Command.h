//
// Created by MasterLogick on 21/06/2020.
//

#ifndef OSU_LASER_CPP_COMMAND_H
#define OSU_LASER_CPP_COMMAND_H

#include "CommandType.h"

namespace osu {
    class Command {
    public:
        CommandType type;
        int startTime;
        int parent;

        void setParent(int parent) {
            this->parent = parent;
        }
    };
}

#endif //OSU_LASER_CPP_COMMAND_H
