//
// Created by MasterLogick on 9/12/20.
//

#ifndef OSU_LASER_CPP_TIMELINE_H
#define OSU_LASER_CPP_TIMELINE_H

#include <list>
#include <vector>
#include <climits>
#include "commands/Command.h"

namespace osu {
    class Timeline {
        std::vector<Command *> cache;
        std::vector<int> timestamps;
        std::vector<int>::iterator nextTimestamp;
        std::vector<Command *>::iterator left;
        std::vector<Command *>::iterator right;
        int lastProcessTime = INT_MIN;
    public:
        void insert(Command *c);

        void pack();

        void process(int time);
    };
}

#endif //OSU_LASER_CPP_TIMELINE_H
