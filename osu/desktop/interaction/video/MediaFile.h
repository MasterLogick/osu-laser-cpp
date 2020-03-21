//
// Created by Masterlogick on 2/25/20.
//

#ifndef OSU_LASER_C_MEDIAFILE_H
#define OSU_LASER_C_MEDIAFILE_H

#include "al.h"
#include "MovieState.h"
#include <thread>

namespace osu {
    class MediaFile {
        std::thread *thr;
        MovieState movState;
    public:
        void draw(int x, int y);

        MediaFile(char *name);

        ~MediaFile();
    };

}

#endif //OSU_LASER_C_MEDIAFILE_H
