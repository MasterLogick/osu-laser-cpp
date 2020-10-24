//
// Created by MasterLogick on 1/25/20.
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

#define SCREEN_INFO_BINDING_POINT 0
namespace osu {
    const char *SCREEN_INFO_BINDING_POINT_NAME{"Screen"};
    Shader *Shader::triangleShader = nullptr;
    Shader *Shader::postProcessorShader = nullptr;
    Shader *Shader::fontShader = nullptr;
    Shader *Shader::spriteDrawingShader = nullptr;
    Shader *Shader::videoDrawingShader = nullptr;

    Shader::Shader(GLuint id, std::pair<char *, int> *binding, std::size_t size) {
        program = id;
        this->bindingPoints = binding;
        pointsCount = size;
    }

    void Shader::initialise() {
        glm::mat4x4 ortho = glm::ortho<float>(0, Graphics::mainScreen->getWidth(), 0, Graphics::mainScreen->getHeight(),
                                              0, MAX_LAYERS_DEPTH);
        GLuint blockVBO;
        glCreateBuffers(1, &blockVBO);
        glNamedBufferData(blockVBO, sizeof(glm::mat4x4) + 4 * sizeof(float), nullptr, GL_STATIC_DRAW);
        glNamedBufferSubData(blockVBO, 0L, sizeof(glm::mat4x4), glm::value_ptr(ortho));
        float width = Graphics::mainScreen->getWidth(), height = Graphics::mainScreen->getHeight();
        glNamedBufferSubData(blockVBO, sizeof(glm::mat4x4), sizeof(float), &width);
        glNamedBufferSubData(blockVBO, sizeof(glm::mat4x4) + sizeof(float), sizeof(float), &height);
        glNamedBufferSubData(blockVBO, sizeof(glm::mat4x4) + 2 * sizeof(float), 2 * sizeof(float), nullptr);
        glBindBufferRange(GL_UNIFORM_BUFFER, SCREEN_INFO_BINDING_POINT, blockVBO, 0,
                          sizeof(glm::mat4x4) + 2 * sizeof(float));

        char *TriangleBackgroundUniforms[] = {(char *) "x", (char *) "y"};
        char *PostProcessorUniforms[] = {(char *) "screenTexture"};
        char *FontUniforms[] = {(char *) "textureSampler", (char *) "Color"};
        char *SpriteDrawingUniforms[] = {(char *) "textureSampler", (char *) "layer", (char *) "alpha"};
        char *VideoUniforms[] = {(char *) "yPlaneSampler", (char *) "uPlaneSampler", (char *) "vPlaneSampler"};

        triangleShader = ShaderLoader::loadShader((char *) "TrianglesBackground", TriangleBackgroundUniforms, 2);
        postProcessorShader = ShaderLoader::loadShader((char *) "PostProcessor", PostProcessorUniforms, 1);
        fontShader = ShaderLoader::loadShader((char *) "Font", FontUniforms, 2);
        spriteDrawingShader = ShaderLoader::loadShader((char *) "RectShader", SpriteDrawingUniforms, 3);
        videoDrawingShader = ShaderLoader::loadShader((char *) "VideoShader", VideoUniforms, 3);

        triangleShader->bindUniform(SCREEN_INFO_BINDING_POINT, const_cast<char *>(SCREEN_INFO_BINDING_POINT_NAME));
        fontShader->bindUniform(SCREEN_INFO_BINDING_POINT, const_cast<char *>(SCREEN_INFO_BINDING_POINT_NAME));
        spriteDrawingShader->bindUniform(SCREEN_INFO_BINDING_POINT, const_cast<char *>(SCREEN_INFO_BINDING_POINT_NAME));
        videoDrawingShader->bindUniform(SCREEN_INFO_BINDING_POINT, const_cast<char *>(SCREEN_INFO_BINDING_POINT_NAME));
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
        for (int i = 0; i < pointsCount; ++i) {
            if (strcmp(bindingPoints[i].first, name) == 0) {
                glUniform1f(bindingPoints[i].second, val);
                break;
            }
        }

    }

    void Shader::uniform(const char *name, int val) {
        for (int i = 0; i < pointsCount; ++i) {
            if (strcmp(bindingPoints[i].first, name) == 0) {
                glUniform1i(bindingPoints[i].second, val);
                break;
            }
        }
    }

    void Shader::uniform3(const char *name, float *val) {
        for (int i = 0; i < pointsCount; ++i) {
            if (strcmp(bindingPoints[i].first, name) == 0) {
                glUniform3fv(bindingPoints[i].second, 1, val);
                break;
            }
        }
    }

    void Shader::uniform4(const char *name, float *val) {
        for (int i = 0; i < pointsCount; ++i) {
            if (strcmp(bindingPoints[i].first, name) == 0) {
                glUniform4fv(bindingPoints[i].second, 1, val);
                break;
            }
        }
    }
}