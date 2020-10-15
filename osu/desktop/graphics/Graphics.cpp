//
// Created by MasterLogick on 1/19/20.
//

#include "Graphics.h"
#include "GameDrawer.h"
#include "Callbacks.h"
#include "../logic/Osu.h"
#include "opengl/GLInfo.h"
#include <mutex>
#include <condition_variable>
#include <glad/glad.h>
#include <iostream>

namespace osu {
    static void initOpenGL();

    static void runThread();

    FPSCounter *Graphics::drawingCounter = new FPSCounter{60};
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
//        Graphics::drawingThread.detach(); todo remove after logic realisation
        if (!drawThreadInitialised)
            _lock.wait(initLocker, []() -> bool {
                return drawThreadInitialised;
            });
    }

    void Graphics::start() {
        drawThreadStartSig = true;
        _lock.notify_all();
    }

    void initOpenGL() {
        glfwSetErrorCallback(errorCallback);
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        Graphics::mainScreen = new GLFWWindow();
        Graphics::mainScreen->setCurrent();
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "GLAD initialisation error" << std::endl;//todo show error message in dialogue window
            exit(1);
        } else {
#ifndef NDEBUG
            std::cout << "GLAD initialised" << std::endl;
#endif
        }
        Graphics::mainScreen->viewport();
        Graphics::mainScreen->setVSync(0);
        Graphics::mainScreen->setWindowShouldCloseCallback(closeCallback);
        Graphics::mainScreen->setKeyCallback(keyCallback);
#ifndef NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(MessageCallback, nullptr);
#endif
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        GLInfo::loadInfo();
    }

    void runThread() {
        initOpenGL();
        GameDrawer::initialise();
        drawThreadInitialised = true;
        _lock.notify_all();
        if (!drawThreadStartSig)
            _lock.wait(runLocker, []() -> bool { return drawThreadStartSig; });
#ifndef NDEBUG
        std::cout << "Drawing loop started" << std::endl;
#endif
        while (!shouldClose) {
            GameDrawer::draw();
            Graphics::drawingCounter->countFPSAndSleep();
            glfwPollEvents();
        }
    }
}