//
// Created by MasterLogick on 2/3/20.
//

#include "BackgroundRootContainer.h"
#include "../../interaction/video/VideoPlayer.h"

namespace osu {

    void BackgroundRootContainer::initialise() {
        triangleBackground.initialise();
        VideoPlayer::initialise();
    }

    void BackgroundRootContainer::draw() {
        draw(0, 0);
    }

    void BackgroundRootContainer::draw(int x, int y) {
        triangleBackground.draw(x, y);
        VideoPlayer::draw(x, y);
    }
}

