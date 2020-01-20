//
// Created by user on 1/19/20.
//

#include <glad/glad.h>
#include "../Callbacks.cpp"

namespace osu {

    void Graphics::initialise() {
        init();
        start();
        //todo wait while graphics thread initialises
    }

    void Graphics::initGraphics() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            //todo handle exception
        }
        glfwSetErrorCallback(errorCallback);
    }

    void Graphics::init() {
        initGraphics();
        mainScreen = new GLFWWindow();
        mainScreen->setCurrent();
        mainScreen->setVSync(1);
        mainScreen->setWindowShouldCloseCallback(closeCallback);
        GameDrawer::initialise();
        UIManager::initialise();
    }

    void Graphics::start() {
        //todo run thread
    }
}