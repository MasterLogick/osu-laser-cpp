//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_COMMANDCONTAINER_H
#define OSU_LASER_CPP_COMMANDCONTAINER_H

#include "commands/Command.h"
#include <vector>

namespace osu {
    class CommandContainer : private std::vector<Command *> {
    private:
        int startTime;
        int endTime;
    public:
        void add(Command *c);

        int getStartTime();

        int getEndTime();

        void pack();

        void process(int time);

        bool empty();
    };
}

#endif //OSU_LASER_CPP_COMMANDCONTAINER_H
