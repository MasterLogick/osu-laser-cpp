//
// Created by MasterLogick on 9/12/20.
//

#ifndef OSU_LASER_CPP_TIMELINE_H
#define OSU_LASER_CPP_TIMELINE_H

#include <list>
#include <vector>
#include "commands/Command.h"

namespace osu {
    class Timeline {
        struct Node {
            int timestamp;
            std::vector<Command *> data;
        };
//        std::vector<Command *> cache;
        std::vector<Node> tl;

        int getTimePoint(int left, int right, int timestamp);

        void insertNode(int pos, int timestamp);

    public:
        void insert(Command *c);

        size_t size();

        void printTimestampsInfo();
    };
}

#endif //OSU_LASER_CPP_TIMELINE_H
