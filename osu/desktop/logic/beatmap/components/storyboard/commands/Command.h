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
        int endTime;
        Event *parent;

        void setParent(Event *parent);

        virtual void process(int time) = 0;

        virtual bool isCompound() = 0;
    };
}

#endif //OSU_LASER_CPP_COMMAND_H
