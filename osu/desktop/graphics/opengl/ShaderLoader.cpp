//
// Created by MasterLogick on 2/5/20.
//
#include "ShaderLoader.h"
#include <cstring>

namespace osu {

    Shader *ShaderLoader::loadShader(char *name, char **uniforms, int count) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        std::string vertexPath = std::string(RESOURCE_ROOT) + "/Shaders/" + name + "/vertex.glsl";
        std::string fragmentPath = std::string(RESOURCE_ROOT) + "/Shaders/" + name + "/fragment.glsl";
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e) {
            //todo catch io_exeption
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        GLuint vertex, fragment;
        vertex = createShader(vShaderCode, GL_VERTEX_SHADER);
        fragment = createShader(fShaderCode, GL_FRAGMENT_SHADER);
        if (vertex == 0 || fragment == 0) {
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            std::cerr << "Shader compilation error" << std::endl;
            return nullptr;
        }
        GLuint program;
        program = linkShaderProgram(vertex, fragment);
        if (program == 0) {
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            glDeleteProgram(program);
            std::cerr << "Shader compilation error" << std::endl;
            return nullptr;
        }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return new Shader(program, getUniformsLocations(program, uniforms, count), count);
    }

    std::pair<char *, int> *ShaderLoader::getUniformsLocations(int program, char **uniforms, int count) {
        std::pair<char *, int> *pairs = new std::pair<char *, int>[count];
        for (int i = 0; i < count; ++i) {
            pairs[i] = std::pair(strdup(uniforms[i]), glGetUniformLocation(program, uniforms[i]));
        }
        return pairs;
    }

    GLuint ShaderLoader::linkShaderProgram(GLint vertexShader, GLint fragmentShader, GLuint geometryShader) {
        GLint success = 1;
        GLchar infoLog[ERROR_BUFFER_SIZE];
        GLuint program;
        program = glCreateProgram();
        if (program == 0) {
            std::cout << "ERROR::PROGRAM_CREATION_ERROR" << std::endl;
            return 0;
        }
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        if (geometryShader != 0)
            glAttachShader(program, geometryShader);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, ERROR_BUFFER_SIZE, nullptr, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR: " << infoLog << std::endl;
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteProgram(program);
            return 0;
        }
        return program;
    }

    GLuint ShaderLoader::createShader(const char *code, GLenum type) {
        GLuint shader = glCreateShader(type);
        if (shader == 0) {
            std::cout << "ERROR::SHADER_CREATION_ERROR" << std::endl;
            return 0;
        }
        glShaderSource(shader, 1, &code, nullptr);
        glCompileShader(shader);
        GLint success;
        GLchar infoLog[ERROR_BUFFER_SIZE];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, ERROR_BUFFER_SIZE, nullptr, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR " << infoLog << std::endl;
            return 0;
        }
        return shader;
    }
}