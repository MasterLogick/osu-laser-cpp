//
// Created by MasterLogick on 2/11/20.
//

#include <string>
#include <boost/lexical_cast.hpp>
#include "ForegroundRootContainer.h"
#include "Font.h"
#include "../Graphics.h"
#include "../../interaction/video/VideoPlayer.h"

#define FONT_SIZE 100
namespace osu {
    static float color[] = {0, 0, 0.5, 1};

    void ForegroundRootContainer::draw(int x, int y) {
        std::wstring str = L"FPS: " + std::to_wstring(Graphics::drawingCounter->getFPS());
        float color[] = {0.5, 0.5, 0.5, 1};
        Font::Exo2_0_Black->draw(const_cast<wchar_t *>(str.c_str()), str.size(), x, y, FONT_SIZE, color);
    }

    void ForegroundRootContainer::draw() {
        draw(0, 0);
    }

    void ForegroundRootContainer::initialise() {
    }

    ForegroundRootContainer::ForegroundRootContainer() {

    }
}