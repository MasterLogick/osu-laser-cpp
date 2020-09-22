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
        if (!empty()) {
            std::sort(begin(), end(), [](Command *a, Command *b) -> bool {
                return a->startTime < b->startTime;
            });
            std::for_each(begin(), end(), [this](Command *c) {
                if (c->endTime > this->endTime) {
                    this->endTime = c->endTime;
                }
            });
            startTime = front()->startTime;
        } else {
            startTime = endTime = 0;
        }
    }

    void CommandContainer::process(int time) {
        std::for_each(begin(), end(), [time](Command *c) {
            if (c->startTime >= time && c->endTime <= time) {
                c->process(time);
            }
        });
    }

    bool CommandContainer::empty() {
        return std::vector<Command *>::empty();
    }
}