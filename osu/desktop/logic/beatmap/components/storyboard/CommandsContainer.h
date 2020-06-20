//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_COMMANDSCONTAINER_H
#define OSU_LASER_CPP_COMMANDSCONTAINER_H

#include "commands/Command.h"

namespace osu {
    class CommandsContainer {
    public:
        void add(Command *c);

        int getStartTime();

        int getEndTime();
    };
}

#endif //OSU_LASER_CPP_COMMANDSCONTAINER_H
