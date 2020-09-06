//
// Created by MasterLogick on 8/31/20.
//

#ifndef OSU_LASER_CPP_TEXTURE_H
#define OSU_LASER_CPP_TEXTURE_H

#include <string>
#include <glad/glad.h>

namespace osu {
    class Texture {
    private:
        int width;
        int height;
        int channels;
        unsigned char *data;
        GLuint texture;
    public:
        Texture();

        explicit Texture(std::string &path);

        explicit Texture(std::string &&path);

        void pushToVideoDrive();
    };
}

#endif //OSU_LASER_CPP_TEXTURE_H
