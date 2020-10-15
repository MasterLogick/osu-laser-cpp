//
// Created by MasterLogick on 10/14/20.
//

#include <glad/glad.h>
#include "GLInfo.h"

#ifndef NDEBUG

#include <iostream>

#endif

namespace osu {
    int GLInfo::MAX_TEXTURE_IMAGE_UNITS = 0;

    int GLInfo::getMaxTextureUnitCount() {
        int count;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &count);
        return count;
    }

    void GLInfo::loadInfo() {
        MAX_TEXTURE_IMAGE_UNITS = getMaxTextureUnitCount();
#ifndef NDEBUG
        std::cout << "Max texture image units: " << MAX_TEXTURE_IMAGE_UNITS << std::endl;
#endif
    }
}