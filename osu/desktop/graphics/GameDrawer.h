//
// Created by MasterLogick on 1/20/20.
//
#ifndef OSU_LASER_C_GAMEDRAWER_H
#define OSU_LASER_C_GAMEDRAWER_H

#include <glad/glad.h>
#include "opengl/Shader.h"
#include "opengl/PostProcessor.h"
#include "UI/UIManager.h"
#include "Graphics.h"

namespace osu {

    class GameDrawer {
    private:
        static PostProcessor postProcessor;

        static void preProcessing();


        static void prepareForGameDrawing();

        static void drawGame();

        static void postProcessing();

        static void render();

    public:
        static void initialise();

        static void draw();
    };
}
#endif