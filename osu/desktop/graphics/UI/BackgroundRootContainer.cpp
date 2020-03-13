//
// Created by MasterLogick on 2/3/20.
//

#include "BackgroundRootContainer.h"
#include "Font.h"
#include "../Graphics.h"

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
        std::wstring str = L"FPS: " + std::to_wstring(Graphics::drawingCounter->getFPS());
        float color[] = {0.5, 0.5, 0.5, 1};
        Font::Exo2_0_Black->draw(const_cast<wchar_t *>(str.c_str()), str.size(), x, y, size, color);
    }
}

