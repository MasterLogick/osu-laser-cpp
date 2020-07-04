//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_COMMANDCONTAINER_H
#define OSU_LASER_CPP_COMMANDCONTAINER_H

#include "commands/Command.h"

namespace osu {
    class CommandContainer {
    public:
        void add(Command *c);

        int getStartTime();

        int getEndTime();
    };
}

#endif //OSU_LASER_CPP_COMMANDCONTAINER_H
