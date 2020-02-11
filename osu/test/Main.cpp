//
// Created by Masterlogick on 2/9/20.
//

#include <locale>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <fwd.hpp>
#include <detail/type_mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.inl>
#include "Main.h"

#define MAX_LAYERS_DEPTH 1000
#define SCREEN_INFO_BINDING_POINT 0
namespace osu {
    bool shouldClose = false;

    int main(int argc, const char **argv, const char **envp) {
        srand(time(nullptr));
        std::locale::global(std::locale("en_US.UTF-8"));
        glfwSetErrorCallback(errorCallback);
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
        GLFWwindow *window = glfwCreateWindow(mode.width, mode.height, "TEST",
                                              glfwGetPrimaryMonitor(),
                                              nullptr);
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "GLAD initialisation error" << std::endl;//todo show error message in dialogue window
            exit(1);
        } else {
#ifndef NDEBUG
            std::cout << "GLAD initialised" << std::endl;
#endif
        }
        glViewport(0, 0, mode.width, mode.height);
        glfwSwapInterval(1);
        glfwSetWindowCloseCallback(window, closeCallback);
#ifndef NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(MessageCallback, nullptr);
#endif
        glm::mat4x4 ortho = glm::ortho<float>(0, mode.width, 0, mode.height,
                                              0, MAX_LAYERS_DEPTH);
        GLuint blockVBO;
        glGenBuffers(1, &blockVBO);
        glBindBuffer(GL_UNIFORM_BUFFER, blockVBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) + 4 * sizeof(float), nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER, 0L, sizeof(glm::mat4x4), glm::value_ptr(ortho));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4), sizeof(float), &mode.width);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) + sizeof(float), sizeof(float), &mode.height);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferRange(GL_UNIFORM_BUFFER, SCREEN_INFO_BINDING_POINT, blockVBO, 0,
                          sizeof(glm::mat4x4) + 2 * sizeof(float));
#ifndef NDEBUG
        std::cout << "Drawing loop started" << std::endl;
#endif
    }

    void errorCallback(int error_code, const char *description) {
        std::cout << "Error code: " << error_code << "\n" << description << std::endl;
    }

#ifndef NDEBUG

    void GLAPIENTRY
    MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                    const void *userParam) {
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cout << "---------------" << std::endl;
        std::cout << "Debug message (" << id << "): " << message << std::endl;

        switch (source) {
            case GL_DEBUG_SOURCE_API:
                std::cout << "Source: API";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                std::cout << "Source: Window System";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                std::cout << "Source: Shader Compiler";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                std::cout << "Source: Third Party";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                std::cout << "Source: Application";
                break;
            case GL_DEBUG_SOURCE_OTHER:
                std::cout << "Source: Other";
                break;
        }
        std::cout << std::endl;

        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                std::cout << "Type: Error";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                std::cout << "Type: Deprecated Behaviour";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                std::cout << "Type: Undefined Behaviour";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                std::cout << "Type: Portability";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                std::cout << "Type: Performance";
                break;
            case GL_DEBUG_TYPE_MARKER:
                std::cout << "Type: Marker";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                std::cout << "Type: Push Group";
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                std::cout << "Type: Pop Group";
                break;
            case GL_DEBUG_TYPE_OTHER:
                std::cout << "Type: Other";
                break;
        }
        std::cout << std::endl;

        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                std::cout << "Severity: high";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                std::cout << "Severity: medium";
                break;
            case GL_DEBUG_SEVERITY_LOW:
                std::cout << "Severity: low";
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                std::cout << "Severity: notification";
                break;
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

#endif

    void closeCallback(GLFWwindow *window) {
        shouldClose = true;
#ifndef NDEBUG
        std::cout << "WindowClose event" << std::endl;
#endif
    }

}

