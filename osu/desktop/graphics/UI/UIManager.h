//
// Created by MasterLogick on 1/20/20.
//
#ifndef OSU_LASER_C_UIMANAGER_H
#define OSU_LASER_C_UIMANAGER_H

#include "BackgroundRootContainer.h"
#include "ForegroundRootContainer.h"

#define MAX_LAYERS_DEPTH 1000
namespace osu {
    class UIManager {
    private:
        static BackgroundRootContainer *background;
        static ForegroundRootContainer *foreground;

    public:

        static void initialise();

        static void drawBackground();

        static void drawForeground();
    };

}
#endif
