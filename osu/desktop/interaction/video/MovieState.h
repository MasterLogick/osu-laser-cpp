//
// Created by Masterlogick on 3/8/20.
//

#ifndef OSU_LASER_C_MOVIESTATE_H
#define OSU_LASER_C_MOVIESTATE_H

#include <chrono>
#include <thread>
#include <string>


extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

#include "VideoState.h"
#include "AudioState.h"


namespace osu {
    class AudioState;

    class VideoState;

    class MovieState {
        AVIOContext *avioctx;
        AVFormatContext *fmtctx;
        bool DisableVideo{false};
        std::chrono::microseconds mClockBase{std::chrono::microseconds::min()};

        AudioState *mAudio;

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

        VideoState *mVideo;

        ~MovieState();

        MovieState(std::string fname);

        bool prepare();

        std::chrono::nanoseconds getMasterClock();

        bool mQuit{false};
    };
}


#endif //OSU_LASER_C_MOVIESTATE_H
