//
// Created by MasterLogick on 7/13/20.
//

#ifndef OSU_LASER_CPP_UNIQUEPTRS_H
#define OSU_LASER_CPP_UNIQUEPTRS_H

#include <memory>

extern "C" {
#include "libavformat/avio.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

namespace osu {
//    struct AVCodecCtxDeleter;
//
//    struct SwsContextDeleter;
//
//    struct AVFrameDeleter;
//
//    struct SwrContextDeleter;
//
//    struct AVIOContextDeleter;
//
//    struct AVFormatCtxDeleter;
//
//    using AVCodecCtxPtr = std::unique_ptr<AVCodecContext, AVCodecCtxDeleter>;
//
//    using SwsContextPtr = std::unique_ptr<SwsContext, SwsContextDeleter>;
//
//    using AVFramePtr = std::unique_ptr<AVFrame, AVFrameDeleter>;
//
//    using SwrContextPtr = std::unique_ptr<SwrContext, SwrContextDeleter>;
//
//    using AVIOContextPtr = std::unique_ptr<AVIOContext, AVIOContextDeleter>;
//
//    using AVFormatCtxPtr = std::unique_ptr<AVFormatContext, AVFormatCtxDeleter>;
    struct AVCodecCtxDeleter {
        void operator()(AVCodecContext *ptr) { avcodec_free_context(&ptr); }
    };

    struct SwsContextDeleter {
        void operator()(SwsContext *ptr) { sws_freeContext(ptr); }
    };

    struct AVFrameDeleter {
        void operator()(AVFrame *ptr) { av_frame_free(&ptr); }
    };

    struct SwrContextDeleter {
        void operator()(SwrContext *ptr) { swr_free(&ptr); }
    };

    struct AVIOContextDeleter {
        void operator()(AVIOContext *ptr) { avio_closep(&ptr); }
    };

    struct AVFormatCtxDeleter {
        void operator()(AVFormatContext *ptr) { avformat_close_input(&ptr); }
    };

    typedef std::unique_ptr<AVCodecContext, AVCodecCtxDeleter> AVCodecCtxPtr;

    typedef std::unique_ptr<SwsContext, SwsContextDeleter> SwsContextPtr;

    typedef std::unique_ptr<AVFrame, AVFrameDeleter> AVFramePtr;

    typedef std::unique_ptr<SwrContext, SwrContextDeleter> SwrContextPtr;

    typedef std::unique_ptr<AVIOContext, AVIOContextDeleter> AVIOContextPtr;

    typedef std::unique_ptr<AVFormatContext, AVFormatCtxDeleter> AVFormatCtxPtr;
}

#endif //OSU_LASER_CPP_UNIQUEPTRS_H
