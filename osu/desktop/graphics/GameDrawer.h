//
// Created by user on 1/20/20.
//
#ifndef OSU_LASER_C_GAMEDRAWER_H
#define OSU_LASER_C_GAMEDRAWER_H

#include <glad/glad.h>
#include "opengl/Shader.h"
#include "opengl/MSAASystem.h"
#include "UI/UIManager.h"
#include "Graphics.h"

#define MSAA_LEVEL 4
namespace osu {
    class GameDrawer {
    private:
        static MSAASystem msaaSystem;

        static void preProcessing() {
            msaaSystem.prepareForDraw();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }


        static void prepareForGameDrawing() {

        }

        static void drawGame() {

        }

        static void postProcessing() {

        }

        static void render() {
            Graphics::mainScreen->swapBuffers();
        }

    public:
        static void initialise() {
            Shader::initialise();
            UIManager::initialise();
            glEnable(GL_BLEND);
        }

        static void draw() {
            preProcessing();
            UIManager::drawBackground();
            prepareForGameDrawing();
            drawGame();
            UIManager::drawForeground();
            postProcessing();
            render();
        }
    };

//    MSAASystem GameDrawer::msaaSystem = MSAASystem(MSAA_LEVEL);
}
#endif