//
// Created by Masterlogick on 3/8/20.
//
#include <iostream>
#include "PacketQueue.h"

namespace osu {
    AVPacket *PacketQueue::getPacket(std::unique_lock<std::mutex> &lock) {
        while (mPackets.empty() && !mFinished)
            mCondVar.wait(lock);
        return mPackets.empty() ? nullptr : &mPackets.front();
    }

    void PacketQueue::pop() {
        AVPacket *pkt = &mPackets.front();
        mTotalSize -= static_cast<unsigned int>(pkt->size);
        av_packet_unref(pkt);
        mPackets.pop_front();
    }

    PacketQueue::~PacketQueue() {
        for (AVPacket &pkt : mPackets)
            av_packet_unref(&pkt);
        mPackets.clear();
        mTotalSize = 0;
    }

    int PacketQueue::sendTo(AVCodecContext *codecctx) {
        std::unique_lock<std::mutex> lock{mMutex};

        AVPacket *pkt{getPacket(lock)};
        if (!pkt) return avcodec_send_packet(codecctx, nullptr);

        const int ret{avcodec_send_packet(codecctx, pkt)};
        if (ret != AVERROR(EAGAIN)) {
            if (ret < 0)
                std::cerr << "Failed to send packet: " << ret << std::endl;
            pop();
        }
        return ret;
    }

    void PacketQueue::setFinished() {
        {
            std::lock_guard<std::mutex> _{mMutex};
            mFinished = true;
        }
        mCondVar.notify_one();
    }

    bool PacketQueue::put(const AVPacket *pkt) {
        {
            std::unique_lock<std::mutex> lock{mMutex};
            if (mTotalSize >= SizeLimit)
                return false;

            mPackets.push_back(AVPacket{});
            if (av_packet_ref(&mPackets.back(), pkt) != 0) {
                mPackets.pop_back();
                return true;
            }

            mTotalSize += static_cast<unsigned int>(mPackets.back().size);
        }
        mCondVar.notify_one();
        return true;
    }

    PacketQueue::PacketQueue(size_t limit) {
        SizeLimit = limit;
    }
}