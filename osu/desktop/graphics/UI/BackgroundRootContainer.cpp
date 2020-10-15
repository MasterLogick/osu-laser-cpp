//
// Created by MasterLogick on 2/3/20.
//

#include "BackgroundRootContainer.h"

namespace osu {

    void BackgroundRootContainer::initialise() {
        triangleBackground.initialise();
//        p.initialise();
    }

    void BackgroundRootContainer::draw() {
        draw(0, 0);
    }

    void BackgroundRootContainer::draw(int x, int y) {
        triangleBackground.draw(x, y);
//        p.draw(x, y);
    }
}

