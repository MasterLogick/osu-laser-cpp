//
// Created by MasterLogick on 3/21/20.
//

#ifndef OSU_LASER_C_VIDEOPLAYER_H
#define OSU_LASER_C_VIDEOPLAYER_H

#include "MediaFile.h"

namespace osu {
    class VideoPlayer {
        static MediaFile *mf;

        static void initAL();

    public:
        static void initialise();

        static void draw(int x, int y);
    };
}

#endif //OSU_LASER_C_VIDEOPLAYER_H
