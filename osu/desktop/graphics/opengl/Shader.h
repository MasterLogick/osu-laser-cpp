//
// Created by user on 1/25/20.
//

#ifndef OSU_LASER_C_SHADER_H
#define OSU_LASER_C_SHADER_H

#include <string>
#include <map>
#include <glad/glad.h>

namespace osu {
    const int SCREEN_INFO_BINDING_POINT = 0;

    class Shader {
    private:
        int program;
        std::map<std::string, int> *uniforms;
        int id;

        void bindUniform(int bindingPoint, char *name);

    public:
        static Shader *triangleShader;

        Shader(int id, std::map<std::string, int> *uniforms);

        void bind();

        GLuint getAttribLocation(const GLchar *name);

        void uniform(const char *name, float val);

        static void initialise();
    };
}

#endif //OSU_LASER_C_SHADER_H
