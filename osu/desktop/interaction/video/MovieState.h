//
// Created by MasterLogick on 7/9/20.
//

#ifndef OSU_LASER_CPP_MOVIESTATE_H
#define OSU_LASER_CPP_MOVIESTATE_H

#include <chrono>
#include <atomic>
#include <memory>
#include <thread>
#include <libavformat/avio.h>
#include <libavformat/avformat.h>
#include "AudioState.h"
#include "VideoState.h"
#include "SyncMaster.h"

namespace osu {
    struct AVIOContextDeleter {
        void operator()(AVIOContext *ptr) { avio_closep(&ptr); }
    };

    using AVIOContextPtr = std::unique_ptr<AVIOContext, AVIOContextDeleter>;

    struct AVFormatCtxDeleter {
        void operator()(AVFormatContext *ptr) { avformat_close_input(&ptr); }
    };

    using AVFormatCtxPtr = std::unique_ptr<AVFormatContext, AVFormatCtxDeleter>;

    class MovieState {
        AVIOContextPtr mIOContext;
        AVFormatCtxPtr mFormatCtx;

        std::chrono::microseconds mClockBase{std::chrono::microseconds::min()};

        AudioState mAudio;

        VideoState mVideo;

        std::thread mParseThread;
        std::thread mAudioThread;

        std::thread mVideoThread;
        std::string mFilename;

        MovieState(std::string fname, SyncMaster syncType);

        ~MovieState();

        static int decode_interrupt_cb(void *ctx);

        bool prepare();

        void setTitle(SDL_Window *window);

        std::chrono::nanoseconds getClock();

        std::chrono::nanoseconds getDuration();

        int streamComponentOpen(unsigned int stream_index);

        int parse_handler();

    public:

        std::atomic<bool> mQuit{false};
        SyncMaster mAVSyncType;

        std::chrono::nanoseconds getMasterClock();

    };
}


#endif //OSU_LASER_CPP_MOVIESTATE_H
