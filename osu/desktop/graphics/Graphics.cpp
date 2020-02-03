//
// Created by user on 1/19/20.
//

#include "Graphics.h"
#include "GameDrawer.h"
#include "UI/UIManager.h"
#include "../Callbacks.h"
#include "../Osu.h"
#include "../interaction/FPSCounter.h"
#include <mutex>
#include <condition_variable>
#include <glad/glad.h>
#include <iostream>

namespace osu {
    FPSCounter drawingCounter = FPSCounter(60);
    GLFWWindow *Graphics::mainScreen = nullptr;
    std::thread *Graphics::drawingThread = nullptr;
    std::condition_variable _lock;
    std::mutex initLock;
    std::mutex runLock;
    std::unique_lock<std::mutex> initLocker(initLock);
    std::unique_lock<std::mutex> runLocker(runLock);
    bool drawThreadInitialised = false;
    bool drawThreadStartSig = false;

    void Graphics::initialise() {
        Graphics::drawingThread = new std::thread(runThread);
//        Graphics::drawingThread.detach(); todo
        if (!drawThreadInitialised)
            _lock.wait(initLocker, []() -> bool {
                return drawThreadInitialised;
            });
    }

    void Graphics::start() {
        drawThreadStartSig = true;
        _lock.notify_all();
    }

    void runThread() {
        glfwSetErrorCallback(errorCallback);
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        Graphics::mainScreen = new GLFWWindow();
        Graphics::mainScreen->setCurrent();
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "GLAD initialisation error" << std::endl;
        } else {
            std::cout << "GLAD initialised" << std::endl;
        }
        Graphics::mainScreen->viewport();
        Graphics::mainScreen->setVSync(1);
        Graphics::mainScreen->setWindowShouldCloseCallback(closeCallback);
#ifndef NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(MessageCallback, nullptr);
//        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
        GameDrawer::initialise();
        drawThreadInitialised = true;
        _lock.notify_all();
        if (!drawThreadStartSig)
            _lock.wait(runLocker, []() -> bool { return drawThreadStartSig; });
        std::cout << "Drawing loop started" << std::endl;
        while (!shouldClose) {
            GameDrawer::draw();
            drawingCounter.countFPSAndSleep();
//            std::cout << "FPS: " << drawingCounter.getFPS() << std::endl;
        }
    }
}