//
// Created by MasterLogick on 21/06/2020.
//

#ifndef OSU_LASER_CPP_COMMAND_H
#define OSU_LASER_CPP_COMMAND_H

#include "../Event.h"
#include "CommandType.h"

namespace osu {
    class Command {
    public:
        CommandType type;
        int startTime;
        Event *parent;

        void setParent(Event *parent);

        bool isCompound();
    };
}

#endif //OSU_LASER_CPP_COMMAND_H
