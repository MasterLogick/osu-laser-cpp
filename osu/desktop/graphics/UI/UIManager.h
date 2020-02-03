//
// Created by user on 1/20/20.
//
#ifndef OSU_LASER_C_UIMANAGER_H
#define OSU_LASER_C_UIMANAGER_H

#include "TriangleBackground.h"
#include "BackgroundRootContainer.h"

#define MAX_LAYERS_DEPTH 1000
namespace osu {
    class UIManager {
    private:
        static BackgroundRootContainer background;
    public:

        static void initialise() {
            UIManager::background.initialise();
        }

        static void drawBackground() {
            UIManager::background.draw();
        }

        static void drawForeground() {

        }
    };

}
#endif
