//
// Created by Masterlogick on 3/24/20.
//

#include <vec4.hpp>
#include "UtillDrawer.h"
#include "Shader.h"

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
        glVertexArrayVertexBuffer(spriteDrawingVAO, spriteDrawingShaderPositionLocation, spriteDrawingVBO, 0, 2 * 2 * sizeof(float));

        glEnableVertexArrayAttrib(spriteDrawingVAO, spriteDrawingShaderTexCordLocation);
        glVertexArrayAttribFormat(spriteDrawingVAO, spriteDrawingShaderTexCordLocation, 2, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(spriteDrawingVAO, spriteDrawingShaderTexCordLocation,
                                  spriteDrawingVBO, 2 * sizeof(float), 2 * 2 * sizeof(float));
    }

    void UtillDrawer::drawRectSprite(glm::vec4 *p1, glm::vec4 *p2, float layer, GLuint spriteTexture) {
        Shader::spriteDrawingShader->bind();
        Shader::spriteDrawingShader->uniform("textureSampler", 0);
        Shader::spriteDrawingShader->uniform("layer", layer);

        glBindVertexArray(spriteDrawingVAO);
        glBindTextureUnit(0, spriteTexture);
        //  +---------------+
        //  | 2           3 |
        //  |               |
        //  |               |
        //  |               |    texture y-axis inverted because of OpenGL implementation specification
        //  |               |
        //  | 0           1 |
        //  +---------------+
        float vertexes[] = {
                p1->x, p1->y, p1->z, 1 - p1->w,
                p2->x, p1->y, p2->z, 1 - p1->w,
                p1->x, p2->y, p1->z, 1 - p2->w,
                p2->x, p2->y, p2->z, 1 - p2->w
        };
        glNamedBufferSubData(spriteDrawingVBO, 0, 4 * 4 * sizeof(float), vertexes);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}