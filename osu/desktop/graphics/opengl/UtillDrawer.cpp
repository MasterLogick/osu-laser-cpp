//
// Created by MasterLogick on 3/24/20.
//

#include <vec4.hpp>
#include <ext.hpp>
#include "UtillDrawer.h"
#include "Shader.h"
#include "GLInfo.h"

namespace osu {
    GLuint UtillDrawer::spriteDrawingVAO = 0;
    GLuint UtillDrawer::spriteDrawingVBO = 0;
    GLuint UtillDrawer::spriteDrawingShaderPositionLocation = 0;
    GLuint UtillDrawer::spriteDrawingShaderTexCordLocation = 0;


    void UtillDrawer::initialise() {
        glCreateVertexArrays(1, &spriteDrawingVAO);

        glCreateBuffers(1, &spriteDrawingVBO);
        glNamedBufferData(spriteDrawingVBO, 4 * 4 * sizeof(float), nullptr, GL_STREAM_DRAW);

        spriteDrawingShaderPositionLocation = Shader::spriteDrawingShader->getAttribLocation("position");
        spriteDrawingShaderTexCordLocation = Shader::spriteDrawingShader->getAttribLocation("texCord");

        glEnableVertexArrayAttrib(spriteDrawingVAO, spriteDrawingShaderPositionLocation);
        glVertexArrayAttribFormat(spriteDrawingVAO, spriteDrawingShaderPositionLocation, 2, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(spriteDrawingVAO, spriteDrawingShaderPositionLocation, spriteDrawingVBO, 0,
                                  2 * 2 * sizeof(float));

        glEnableVertexArrayAttrib(spriteDrawingVAO, spriteDrawingShaderTexCordLocation);
        glVertexArrayAttribFormat(spriteDrawingVAO, spriteDrawingShaderTexCordLocation, 2, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(spriteDrawingVAO, spriteDrawingShaderTexCordLocation,
                                  spriteDrawingVBO, 2 * sizeof(float), 2 * 2 * sizeof(float));
    }

    void UtillDrawer::drawRectSprite(float x1, float y1, float x2, float y2, float layer, GLuint spriteTexture) {
        Shader::spriteDrawingShader->bind();
        Shader::spriteDrawingShader->uniform("textureSampler", GLInfo::MAX_TEXTURE_IMAGE_UNITS - 1);
        Shader::spriteDrawingShader->uniform("layer", layer);

        glBindVertexArray(spriteDrawingVAO);
        glBindTextureUnit(GLInfo::MAX_TEXTURE_IMAGE_UNITS - 1, spriteTexture);
        //  +---------------+
        //  | 2           3 |
        //  |               |
        //  |               |
        //  |               |    texture y-axis inverted because of OpenGL implementation specification
        //  |               |
        //  | 0           1 |
        //  +---------------+
        float vertexes[] = {
                x1, y1,
                x2, y1,
                x1, y2,
                x2, y2
        };
        glNamedBufferSubData(spriteDrawingVBO, 0, 2 * 2 * sizeof(float), vertexes);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}