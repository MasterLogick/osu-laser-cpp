//
// Created by Masterlogick on 3/24/20.
//

#ifndef OSU_LASER_C_UTILLDRAWER_H
#define OSU_LASER_C_UTILLDRAWER_H

#include <glad/glad.h>
#include <vec4.hpp>

namespace osu {

    class UtillDrawer {
    private:
        static GLuint spriteDrawingVAO;
        static GLuint spriteDrawingVBO;
        static GLuint spriteDrawingShaderPositionLocation;
        static GLuint spriteDrawingShaderTexCordLocation;
    public:
        static void initialise();

        static void drawRectSprite(glm::vec4 *p1, glm::vec4 *p2, float layer, GLuint spriteTexture);
    };
}

#endif //OSU_LASER_C_UTILLDRAWER_H
