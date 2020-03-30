//
// Created by Masterlogick on 2/25/20.
//

#ifndef OSU_LASER_C_MEDIAFILE_H
#define OSU_LASER_C_MEDIAFILE_H

#include "MovieState.h"
#include <thread>
#include <mutex>
#include <condition_variable>

namespace osu {
    class MediaFile {
    private:
        std::condition_variable _lock;
        std::mutex initLock;
        std::unique_lock<std::mutex> initLocker;
        bool initialised{false};

        std::thread *thr;
        MovieState movState;
    public:
        void initialise();

        void draw(int x, int y);

        MediaFile(char *name);

        ~MediaFile();
    };

}

#endif //OSU_LASER_C_MEDIAFILE_H
