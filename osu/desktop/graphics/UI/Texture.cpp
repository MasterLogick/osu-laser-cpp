//
// Created by MasterLogick on 8/31/20.
//

#include "../stb_image.h"
#include "Texture.h"

namespace osu {

    Texture::Texture(const char *path) {
        data = stbi_load(path, &width, &height, &channels, 4);
    }

    void Texture::pushToVideoDrive() {
        if (!data) {
            //todo throw bad_texture_format exception
        }
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    Texture::Texture() : data(nullptr) {
    }
}