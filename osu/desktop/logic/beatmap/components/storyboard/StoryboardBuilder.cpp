//
// Created by MasterLogick on 6/18/20.
//

#include "StoryboardBuilder.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace osu {
    StoryboardBuilder::StoryboardBuilder(path dir) : dir(dir) {
    }

    void StoryboardBuilder::addEvent(Event *event) {
        events.push_back(event);
    }

    void StoryboardBuilder::addEventCommandContainer(CompoundCommand *container) {
        commandContainers.push_back(container);
    }

    void StoryboardBuilder::setRootPath(path dir) {
        this->dir = dir;
    }

    Storyboard *StoryboardBuilder::build() {
        std::sort(events.begin(), events.end(),
                  [](Event *e1, Event *e2) -> bool {
                      return e1->startTime < e2->startTime;
                  });
        std::sort(commandContainers.begin(), commandContainers.end(),
                  [](CommandContainer *c1, CommandContainer *c2) -> bool {
                      return c1->getStartTime() < c2->getStartTime();
                  });

    }
}