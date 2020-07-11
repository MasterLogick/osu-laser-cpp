//
// Created by MasterLogick on 7/8/20.
//

#ifndef OSU_LASER_CPP_PACKETQUEUE_H
#define OSU_LASER_CPP_PACKETQUEUE_H

#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <deque>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavcodec/packet.h>
}
namespace osu {
    class PacketQueue {
        std::mutex mMutex;
        std::condition_variable mCondVar;
        std::deque<AVPacket> mPackets;
        size_t mTotalSize{0};
        bool mFinished{false};
        size_t SizeLimit;

        AVPacket *getPacket(std::unique_lock<std::mutex> &lock);

        void pop();

    public:
        explicit PacketQueue(size_t SizeLimits);

        ~PacketQueue();

        int sendTo(AVCodecContext *codecctx);

        void setFinished();

        bool put(const AVPacket *pkt);
    };
}


#endif //OSU_LASER_CPP_PACKETQUEUE_H
