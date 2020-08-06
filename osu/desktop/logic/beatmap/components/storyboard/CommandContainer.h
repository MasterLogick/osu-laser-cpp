//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_COMMANDCONTAINER_H
#define OSU_LASER_CPP_COMMANDCONTAINER_H

#include "commands/Command.h"
#include <list>

namespace osu {
    class CommandContainer : private std::list<Command *> {
    private:
        int startTime;
        int endTime;
        std::list<Command *>::iterator next;
        Command *current;
        int nextTime;
    public:
        void add(Command *c);

        int getStartTime();

        int getEndTime();

        void pack();

        Command *get(int time);
    };
}

#endif //OSU_LASER_CPP_COMMANDCONTAINER_H
