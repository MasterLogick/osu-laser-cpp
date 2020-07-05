//
// Created by MasterLogick on 20/06/2020.
//

#include "CommandContainer.h"
#include <algorithm>

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

    void CommandContainer::commit() {
        sort([](Command *a, Command *b) -> bool {
            return a->startTime < b->startTime || (a->startTime == b->endTime && a->endTime < b->endTime);
        });
        std::for_each(begin(), end(), [this](Command *c) {
            if (c->endTime > this->endTime) {
                this->endTime = c->endTime;
            }
        });
    }
}