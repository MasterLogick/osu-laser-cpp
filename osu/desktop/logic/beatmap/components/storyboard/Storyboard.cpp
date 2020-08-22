//
// Created by MasterLogick on 6/18/20.
//

#include "Storyboard.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace osu {

    void Storyboard::addEvent(Event *event) {
        events.push_back(event);
    }

    void Storyboard::addEventCommandContainer(CompoundCommand *container) {
        containers.push_back(container);
//        std::cout << "cc" << std::endl;
    }

    void Storyboard::cacheSprites() {
        for (int i = 0, j = events.size(); i < j; ++i) {
            Event *e = events[i];
            if (e->eventType == EventType::ETAnimation) {
                Animation *anim = static_cast<Animation *>(e);

            }
        }
    }

    void Storyboard::setDirectory(std::string dir) {
        this->dir = dir;
    }
}