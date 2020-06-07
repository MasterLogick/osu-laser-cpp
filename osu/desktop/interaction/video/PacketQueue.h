//
// Created by MasterLogick on 3/8/20.
//

#ifndef OSU_LASER_C_PACKETQUEUE_H
#define OSU_LASER_C_PACKETQUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>
extern "C"{
#include <libavcodec/avcodec.h>
}
namespace osu{

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
        ~PacketQueue();

        PacketQueue(size_t limit);

        int sendTo(AVCodecContext *codecctx);

        void setFinished();

        bool put(const AVPacket *pkt);
    };
}


#endif //OSU_LASER_C_PACKETQUEUE_H
