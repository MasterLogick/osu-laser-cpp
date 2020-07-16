//
// Created by MasterLogick on 1/19/20.
//
#ifndef OSU_LASER_C_GRAPHICS_H
#define OSU_LASER_C_GRAPHICS_H

#include <chrono>
#include <thread>
#include "GLFWWindow.h"
#include "../interaction/FPSCounter.h"

namespace osu {

    extern FPSCounter drawingCounter;

    class Graphics {
    public:
        static FPSCounter *drawingCounter;
        static std::thread *drawingThread;
        static GLFWWindow *mainScreen;

        static void initialise();

        static void start();
    };
}

#endif