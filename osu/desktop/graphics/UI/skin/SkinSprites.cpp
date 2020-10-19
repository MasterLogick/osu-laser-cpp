//
// Created by MasterLogick on 10/14/20.
//

#include <platformDependentDefines.h>
#include <string>
#include "SkinSprites.h"
#include "../../stb_image.h"

namespace osu {
    GLuint SkinSprites::CircleSprite = 0;
    GLuint SkinSprites::HitCircleOverlay = 0;

    void SkinSprites::loadSprite(GLuint *var, char *name) {
        glCreateTextures(GL_TEXTURE_2D, 1, var);
        int width, height, channels;
        unsigned char *data = stbi_load((std::string(SKIN_PATH) + name + "@2x.png").c_str(), &width, &height,
                                        &channels, 4);
        if (data) {
            glTextureStorage2D(*var, 1, GL_RGBA8, width, height);
            glTextureSubImage2D(*var, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            //todo throw bad_texture_format exception
        }
        stbi_image_free(data);
    }

    void SkinSprites::loadSprites() {
        loadSprite(&CircleSprite, "hitcircle");
        loadSprite(&HitCircleOverlay, "hitcircleoverlay");
    }
}