//
// Created by MasterLogick on 9/12/20.
//

#ifndef OSU_LASER_CPP_TIMELINE_H
#define OSU_LASER_CPP_TIMELINE_H

#include <list>
#include <vector>
#include "commands/CompoundCommand.h"

namespace osu {
    class Timeline {
        struct Node {
            int timestamp;
            std::vector<CompoundCommand *> data;
        };

        std::list<Node> tl;

    public:
        void insert(CompoundCommand *c);

        size_t size();

        void print();
    };
}

#endif //OSU_LASER_CPP_TIMELINE_H
