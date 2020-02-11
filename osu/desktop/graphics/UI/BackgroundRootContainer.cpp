//
// Created by MasterLogick on 2/3/20.
//

#include "BackgroundRootContainer.h"
#include "Font.h"

namespace osu {
    int BackgroundRootContainer::size = 100;

    void BackgroundRootContainer::initialise() {
        triangleBackground.initialise();
    }

    void BackgroundRootContainer::draw() {
        draw(0, 0);
    }

    void BackgroundRootContainer::draw(int x, int y) {
        triangleBackground.draw(x, y);
//        wchar_t str[] = L"Java is the best language";
//        float color[] = {0.5, 0.5, 0.5, 1.0};
//        Font::Exo2_0_Black->draw(str, sizeof(str)/ sizeof(wchar_t), x, y, size, color);
    }
}

