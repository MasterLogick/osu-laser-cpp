//
// Created by MasterLogick on 2/25/20.
//
#include <iostream>
#include <thread>
#include <cmath>
#include <deque>
#include "MediaFile.h"

namespace osu {

    inline constexpr int64_t operator "" _i64(unsigned long long int n) noexcept { return static_cast<int64_t>(n); }

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

    MediaFile::MediaFile(char *name) : movState(name), initLocker(initLock) {
        MediaFile *mf = this;
        thr = new std::thread([mf, name]() {
            if (!mf->initialised)
                mf->_lock.wait(mf->initLocker, [mf]() -> bool { return mf->initialised; });
#ifndef NDEBUG
            av_log_set_level(AV_LOG_ERROR);
#endif
            // Register all formats and codecs
#if !(LIBAVFORMAT_VERSION_INT >= AV_VERSION_INT(58, 9, 100))
            av_register_all();
#endif
            // Initialize networking protocols
            avformat_network_init();
            mf->movState.prepare();
            while (!mf->movState.mQuit) {
                mf->movState.mVideo->updateVideo();
            }
            //todo stop playing
        });
    }

    MediaFile::~MediaFile() {
        thr->join();
        delete thr;
    }

    void MediaFile::draw(int x, int y) {
        movState.mVideo->draw(x, y);
    }

    void MediaFile::initialise() {
        movState.mVideo->initialise();
        initialised = true;
        _lock.notify_one();
    }
}
