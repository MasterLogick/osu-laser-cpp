//
// Created by MasterLogick on 20/06/2020.
//

#include "CommandContainer.h"
#include <algorithm>
#include <climits>

namespace osu {

    void CommandContainer::add(Command *c) {
        push_back(c);
    }

    int CommandContainer::getStartTime() {
        return startTime;
    }

    int CommandContainer::getEndTime() {
        return endTime;
    }

    void CommandContainer::pack() {
        sort([](Command *a, Command *b) -> bool {
            return a->startTime < b->startTime;
        });
        std::for_each(begin(), end(), [this](Command *c) {
            if (c->endTime > this->endTime) {
                this->endTime = c->endTime;
            }
        });
        next = begin();
        current = *next;
        nextTime = (*(next++))->startTime;
    }

    Command *CommandContainer::get(int time) {
        if (nextTime <= time) {
            current = *next;
            if ((next++) == end()) {
                nextTime = INT_MAX;
            } else {
                nextTime = (*(next))->startTime;
            }
        }
        return current;
    }
}