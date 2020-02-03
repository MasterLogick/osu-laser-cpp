//
// Created by user on 1/20/20.
//
#include "Callbacks.h"
#include "Osu.h"
#include <iostream>
#include <glad/glad.h>

namespace osu {
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
        /*std::string severityStr;
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH :
                severityStr = "HIGH SEVERITY";
                break;
            case GL_DEBUG_SEVERITY_LOW :
                severityStr = "LOW SEVERITY";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM :
                severityStr = "MEDIUM SEVERITY";
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                severityStr = "INFO";
                break;
        }
        std::string typeStr;
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                typeStr = "ERROR";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                typeStr = "DEPRECATED BEHAVIOR";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                typeStr = "UNDEFINED BEHAVIOR";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                typeStr = "TYPE PORTABILITY";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                typeStr = "TYPE PERFORMANCE";
                break;
            case GL_DEBUG_TYPE_OTHER :
                typeStr = "TYPE OTHER";
                break;
        }
        fprintf(stderr, "GL CALLBACK: %s: %s: %s\n",
                typeStr.c_str(), severityStr.c_str(), message);*/

    }

#endif

    void closeCallback(GLFWwindow *window) {
        shouldClose = true;
#ifndef NDEBUG
        std::cout << "WindowClose event" << std::endl;
#endif
    }
}