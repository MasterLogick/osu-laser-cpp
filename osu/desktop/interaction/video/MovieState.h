//
// Created by MasterLogick on 7/9/20.
//

#ifndef OSU_LASER_CPP_MOVIESTATE_H
#define OSU_LASER_CPP_MOVIESTATE_H

#include <chrono>
#include <atomic>
#include <memory>
#include <thread>
#include "AudioState.h"
#include "VideoState.h"
#include "SyncMaster.h"
#include "UniquePtrs.h"

namespace osu {
    class AudioState;

    class VideoState;

    class MovieState {
        AVIOContextPtr mIOContext;
        AVFormatCtxPtr mFormatCtx;

        std::chrono::microseconds mClockBase{std::chrono::microseconds::min()};

        std::thread mParseThread;

        std::thread mAudioThread;

        std::thread mVideoThread;
        std::string mFilename;

        static int decode_interrupt_cb(void *ctx);

        std::chrono::nanoseconds getClock();

        std::chrono::nanoseconds getDuration();

        int streamComponentOpen(unsigned int stream_index);

        int parse_handler();

    public:

        AudioState mAudio;

        VideoState mVideo;

        std::atomic<bool> mQuit{false};

        SyncMaster mAVSyncType;

        std::chrono::nanoseconds getMasterClock();

        bool prepare();

        void start();

        MovieState(std::string fname, SyncMaster syncType);

        ~MovieState();
    };
}


#endif //OSU_LASER_CPP_MOVIESTATE_H
