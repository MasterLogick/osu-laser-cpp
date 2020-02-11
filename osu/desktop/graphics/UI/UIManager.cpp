//
// Created by MasterLogick on 1/27/20.
//

#include "UIManager.h"

namespace osu {
    BackgroundRootContainer *UIManager::background = new BackgroundRootContainer();
    ForegroundRootContainer *UIManager::foreground = new ForegroundRootContainer();

    void UIManager::drawForeground() {
        UIManager::foreground->draw();
    }

    void UIManager::drawBackground() {
        UIManager::background->draw();
    }

    void UIManager::initialise() {
        UIManager::background->initialise();
        UIManager::foreground->initialise();
    }
}