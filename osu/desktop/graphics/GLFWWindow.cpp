//
// Created by user on 1/18/20.
//

#include "GLFWWindow.h"
#include "../interaction/Properties.h"
#include <iostream>
#include <glad/glad.h>

namespace osu {
    void GLFWWindow::printPrimaryMonitorVideoModes() {
        int count = 0;
        const GLFWvidmode *vidMode = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
        for (int i = 0; i < count; ++i) {
            GLFWvidmode mode = vidMode[i];
            std::cout << mode.width << "x" << mode.height << "@" << mode.refreshRate << std::endl;
        }
    }

    GLFWWindow::GLFWWindow() {
#ifndef NDEBUG
        GLFWWindow::printPrimaryMonitorVideoModes();
#endif
        if (Properties::VideoMode->exists("Width") && Properties::VideoMode->exists("Height") &&
            Properties::VideoMode->exists("RefreshRate") && Properties::VideoMode->exists("Type")) {
            const GLFWvidmode mode = *glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwDefaultWindowHints();
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_REFRESH_RATE, mode.refreshRate);
#ifndef NDEBUG
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
            window = glfwCreateWindow(mode.width, mode.height, (const char *) (*Properties::Title),
                                      glfwGetPrimaryMonitor(),
                                      nullptr);
            width = mode.width;
            height = mode.height;
            (*Properties::VideoMode)["Width"] = width;
            (*Properties::VideoMode)["Height"] = height;
            (*Properties::VideoMode)["RefreshRate"] = mode.refreshRate;
            (*Properties::VideoMode)["Type"] = TYPE_FULLSCREEN;
        } else {
            int count = 0;
            const GLFWvidmode *vidMode = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
            bool hasVideoMode = false;
            const GLFWvidmode *mode;
            for (int i = 0; i < count; ++i) {
                if (vidMode[i].width == (int(*Properties::VideoMode)["Width"])&&
                vidMode[i].height == (int(*Properties::VideoMode)["Height"])&&
                vidMode[i].refreshRate == (int(*Properties::VideoMode)["RefreshRate"])){
                    hasVideoMode = true;
                    mode = &vidMode[i];
                }
            }
            int refreshRate, type = TYPE_WINDOW;
            if (hasVideoMode) {
                width = mode->width;
                height = mode->height;
                refreshRate = mode->refreshRate;
            } else {
                width = vidMode[count - 1].width;
                height = vidMode[count - 1].height;
                refreshRate = vidMode[count - 1].refreshRate;
            }
            const char *title = (const char *) (*Properties::Title);
            glfwDefaultWindowHints();
#ifndef NDEBUG
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            switch ((int) (*Properties::VideoMode)["Type"]) {
                case TYPE_BORDERLESS:
                    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
                    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
                    type = TYPE_BORDERLESS;
                    break;
                case TYPE_FULLSCREEN:
                    glfwWindowHint(GLFW_REFRESH_RATE, refreshRate);
                    window = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), nullptr);
                    type = TYPE_FULLSCREEN;
                    break;
                case TYPE_WINDOW:
                    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
                    type = TYPE_WINDOW;
                    break;
            }
            (*Properties::VideoMode)["Width"] = width;
            (*Properties::VideoMode)["Height"] = height;
            (*Properties::VideoMode)["RefreshRate"] = refreshRate;
            (*Properties::VideoMode)["Type"] = type;
        }

        //todo chech if window == nullptr
    }

    GLFWWindow::~GLFWWindow() {
        //todo
    }

    void GLFWWindow::setCurrent() {
        glfwMakeContextCurrent(window);
    }

    void GLFWWindow::setVSync(int val) {
        glfwSwapInterval(val);
    }

    void GLFWWindow::showWindow() {
        glfwShowWindow(window);
    }


    void GLFWWindow::hideWindow() {
        glfwHideWindow(window);
    }

    void GLFWWindow::swapBuffers() {
        glfwSwapBuffers(window);
    }

    bool GLFWWindow::isWindowShouldClose() {
        return glfwWindowShouldClose(window);
    }

    void GLFWWindow::shouldClose() {
        glfwSetWindowShouldClose(window, true);
    }

    void GLFWWindow::freeCallbacks() {
        glfwSetWindowPosCallback(window, nullptr);
        glfwSetWindowSizeCallback(window, nullptr);
        glfwSetWindowCloseCallback(window, nullptr);
        glfwSetWindowRefreshCallback(window, nullptr);
        glfwSetWindowFocusCallback(window, nullptr);
        glfwSetWindowIconifyCallback(window, nullptr);
        glfwSetWindowMaximizeCallback(window, nullptr);
        glfwSetFramebufferSizeCallback(window, nullptr);
        glfwSetWindowContentScaleCallback(window, nullptr);
        glfwSetKeyCallback(window, nullptr);
        glfwSetCharCallback(window, nullptr);
        glfwSetCharModsCallback(window, nullptr);
        glfwSetMouseButtonCallback(window, nullptr);
        glfwSetCursorPosCallback(window, nullptr);
        glfwSetCursorEnterCallback(window, nullptr);
        glfwSetScrollCallback(window, nullptr);
        glfwSetDropCallback(window, nullptr);
    }


    void GLFWWindow::setKeyCallback(GLFWkeyfun callback) {
        glfwSetKeyCallback(window, callback);
    }

    void GLFWWindow::setMouseCallback(GLFWcursorposfun callback) {
        glfwSetCursorPosCallback(window, callback);
    }

    void GLFWWindow::grabMouse() {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    }

    void GLFWWindow::releaseMouse() {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void GLFWWindow::setMousePos(double xpos, double ypos) {
        glfwSetCursorPos(window, xpos, ypos);
    }

    int GLFWWindow::getWidth() {
        return width;
    }

    int GLFWWindow::getHeight() {
        return height;
    }

    void GLFWWindow::viewport() {
//        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
    }

    int GLFWWindow::getX() {
        int x;
        glfwGetWindowPos(window, &x, nullptr);
        return x;
    }

    int GLFWWindow::getY() {
        int y;
        glfwGetWindowPos(window, &y, nullptr);
        return y;
    }

    void GLFWWindow::setWindowShouldCloseCallback(GLFWwindowclosefun callback) {
        glfwSetWindowCloseCallback(window, callback);
    }

}