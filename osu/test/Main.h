//
// Created by Masterlogick on 2/9/20.
//

#ifndef OSU_LASER_C_MAIN_H
#define OSU_LASER_C_MAIN_H

namespace osu {
    int main(int argc, const char **argv, const char **envp);

    void errorCallback(int error_code, const char *description);

    void closeCallback(GLFWwindow *window);

#ifndef NDEBUG

    void GLAPIENTRY
    MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                    const void *userParam);

#endif
}


#endif //OSU_LASER_C_MAIN_H
