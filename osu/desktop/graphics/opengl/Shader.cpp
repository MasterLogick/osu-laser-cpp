//
// Created by user on 1/25/20.
//

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.inl>
#include <iostream>
#include "Shader.h"
#include "glad/glad.h"
#include "ShaderLoader.h"
#include "mat4x4.hpp"
#include "../Graphics.h"
#include "../UI/UIManager.h"

namespace osu {
    Shader *Shader::triangleShader = nullptr;
    Shader *Shader::postProcessor = nullptr;

    osu::Shader::Shader(int id, std::map<std::string, int> *uniforms) {
        program = id;
        this->uniforms = uniforms;
    }

    void Shader::initialise() {
        glm::mat4x4 ortho = glm::ortho<float>(0, Graphics::mainScreen->getWidth(), 0, Graphics::mainScreen->getHeight(),
                                              0, MAX_LAYERS_DEPTH);
        GLuint blockVBO;
        glGenBuffers(1, &blockVBO);
        glBindBuffer(GL_UNIFORM_BUFFER, blockVBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) + 2 * sizeof(float), nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_UNIFORM_BUFFER, 0L, sizeof(glm::mat4x4), glm::value_ptr(ortho));
        float width = Graphics::mainScreen->getWidth(), height = Graphics::mainScreen->getHeight();
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4), sizeof(float), &width);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) + sizeof(float), sizeof(float), &height);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferRange(GL_UNIFORM_BUFFER, SCREEN_INFO_BINDING_POINT, blockVBO, 0,
                          sizeof(glm::mat4x4) + 2 * sizeof(float));
        char *c[2];
        c[0] = (char *) "x";
        c[1] = (char *) "y";
        triangleShader = ShaderLoader::loadShader((char *) "TrianglesBackground", c, 2);
        postProcessor = ShaderLoader::loadShader((char *) "PostProcessor", nullptr, 0);
        triangleShader->
                bindUniform(SCREEN_INFO_BINDING_POINT,
                            (char *) "Screen");
    }

    void Shader::bind() {
        glUseProgram(program);
    }

    void Shader::bindUniform(int bindingPoint, char *name) {
        GLuint index = glGetUniformBlockIndex(program, name);
        glUniformBlockBinding(program, index, bindingPoint);

    }

    GLuint Shader::getAttribLocation(const GLchar *name) {
        return glGetAttribLocation(program, name);
    }

    void Shader::uniform(const char *name, float val) {
        glUniform1f(uniforms->find(name)->second, val);
    }

    void Shader::uniform(const char *name, int val) {
        glUniform1i(uniforms->find(name)->second, val);
    }
}