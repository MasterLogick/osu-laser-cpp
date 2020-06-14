//
// Created by MasterLogick on 1/23/20.
//
#ifndef OSU_LASER_C_CALLBACKS_H
#define OSU_LASER_C_CALLBACKS_H

#include <glad/glad.h>
#include "GLFWWindow.h"

namespace osu {
    void errorCallback(int error_code, const char *description);

    void closeCallback(GLFWwindow *window);

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

#ifndef NDEBUG

    void GLAPIENTRY
    MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                    const void *userParam);

#endif
}
#endif
