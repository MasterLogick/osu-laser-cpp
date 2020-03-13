//
// Created by MasterLogick on 1/27/20.
//

#include "GameDrawer.h"
#include "UI/Font.h"
#include "../interaction/video/MediaFile.h"

#define MSAA_LEVEL 4
namespace osu {
    PostProcessor GameDrawer::postProcessor = PostProcessor(MSAA_LEVEL);

    void GameDrawer::preProcessing() {
        postProcessor.preparePostProcessor();
        glEnable(GL_BLEND);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void GameDrawer::prepareForGameDrawing() {

    }

    void GameDrawer::drawGame() {

    }

    void GameDrawer::postProcessing() {
        postProcessor.postProcess(0);// 0 is id of main window framebuffer
    }

    void GameDrawer::render() {
        Graphics::mainScreen->swapBuffers();
    }

    void GameDrawer::initialise() {
        Shader::initialise();
        Font::initialise();
        postProcessor.initialise(Graphics::mainScreen->getWidth(), Graphics::mainScreen->getHeight());
        UIManager::initialise();
    }

    void GameDrawer::draw() {
        preProcessing();
        UIManager::drawBackground();
        prepareForGameDrawing();
        drawGame();
        UIManager::drawForeground();
        postProcessing();
        render();
    }
}