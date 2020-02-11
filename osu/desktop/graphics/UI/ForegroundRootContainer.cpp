//
// Created by Masterlogick on 2/11/20.
//

#include <string>
#include <boost/lexical_cast.hpp>
#include "ForegroundRootContainer.h"
#include "Font.h"
#include "../Graphics.h"

namespace osu {
    static float color[] = {0, 0, 0.5, 1};

    void ForegroundRootContainer::draw(int x, int y) {
        fps = L"FPS: " + boost::lexical_cast<std::wstring>(Graphics::drawingCounter->getFPS());
        Font::Exo2_0_Black->draw((wchar_t *) (fps.c_str()), fps.size(), x, y, 50, color);
    }

    void ForegroundRootContainer::draw() {
        draw(0, 0);
    }

    void ForegroundRootContainer::initialise() {
    }

    ForegroundRootContainer::ForegroundRootContainer() {

    }
}