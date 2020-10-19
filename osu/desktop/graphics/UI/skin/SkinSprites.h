//
// Created by MasterLogick on 10/14/20.
//

#ifndef OSU_LASER_CPP_SKINSPRITES_H
#define OSU_LASER_CPP_SKINSPRITES_H

#include <glad/glad.h>

namespace osu {
    class SkinSprites {
        static void loadSprite(GLuint *var, char *name);

    public:
        static GLuint CircleSprite;
        static GLuint HitCircleOverlay;

        static void loadSprites();
    };
}

#endif //OSU_LASER_CPP_SKINSPRITES_H
