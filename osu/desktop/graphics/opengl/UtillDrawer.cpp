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
    float initVector[] = {0, 0, 0, 0, 0, 0, 0, 0, //vertexes data
                          1, 0, 0, 0, 1, 1, 0, 1}; //texCord data (y-cords inverted. thanks to opengl)


    void UtillDrawer::initialise() {
        glCreateVertexArrays(1, &spriteDrawingVAO);

        glCreateBuffers(1, &spriteDrawingVBO);

        glNamedBufferData(spriteDrawingVBO, 4 * 4 * sizeof(float), initVector, GL_STREAM_DRAW);

        GLuint spriteDrawingShaderPositionLocation = Shader::spriteDrawingShader->getAttribLocation("position");
        GLuint spriteDrawingShaderTexCordLocation = Shader::spriteDrawingShader->getAttribLocation("texCord");

        glEnableVertexArrayAttrib(spriteDrawingVAO, spriteDrawingShaderPositionLocation);
        glVertexArrayAttribFormat(spriteDrawingVAO, spriteDrawingShaderPositionLocation, 2, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(spriteDrawingVAO, spriteDrawingShaderPositionLocation, spriteDrawingVBO, 0,
                                  2 * sizeof(float));

        glEnableVertexArrayAttrib(spriteDrawingVAO, spriteDrawingShaderTexCordLocation);
        glVertexArrayAttribFormat(spriteDrawingVAO, spriteDrawingShaderTexCordLocation, 2, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(spriteDrawingVAO, spriteDrawingShaderTexCordLocation,
                                  spriteDrawingVBO, 4 * 2 * sizeof(float), 2 * sizeof(float));
    }

    void UtillDrawer::drawRectSprite(float x1, float y1, float x2, float y2, float layer, GLuint spriteTexture,
                                     float transparency) {
        Shader::spriteDrawingShader->bind();
        Shader::spriteDrawingShader->uniform("textureSampler", GLInfo::MAX_TEXTURE_IMAGE_UNITS - 1);
        Shader::spriteDrawingShader->uniform("layer", layer);
        Shader::spriteDrawingShader->uniform("alpha", (float) transparency);

        glBindVertexArray(spriteDrawingVAO);
        glBindTextureUnit(GLInfo::MAX_TEXTURE_IMAGE_UNITS - 1, spriteTexture);
        //
        //     4,5,6,7         0,1,2,3
        //     +---------------+
        //     | 3           2 |
        //     |               |
        //     |   (sprite)    |               (x,y,s,t)
        //     |               |
        //     |               |
        //     | 1           0 |
        //     +---------------+
        //     12,13,14,15     8,9,10,11
        //
        float vertexes[] = {
                x2, y2,
                x1, y2,
                x2, y1,
                x1, y1
        };
        glNamedBufferSubData(spriteDrawingVBO, 0, 4 * 2 * sizeof(float), vertexes);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}