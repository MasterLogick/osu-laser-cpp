//
// Created by MasterLogick on 10/14/20.
//

#ifndef OSU_LASER_CPP_GLINFO_H
#define OSU_LASER_CPP_GLINFO_H

namespace osu {
    class GLInfo {

        static int getMaxTextureUnitCount();

    public:

        static int MAX_TEXTURE_IMAGE_UNITS;

        static void loadInfo();
    };
}
#endif //OSU_LASER_CPP_GLINFO_H
