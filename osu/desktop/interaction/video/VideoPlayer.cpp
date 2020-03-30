//
// Created by Masterlogick on 3/21/20.
//
#include <iostream>
#include "VideoPlayer.h"
#include "../audio/common/alhelpers.h"
#include "../audio/AudioSystem.h"

namespace osu {
    MediaFile *VideoPlayer::mf = nullptr;

    void VideoPlayer::draw(int x, int y) {
        mf->draw(x, y);
    }

    void VideoPlayer::initialise() {
        initAL();
        mf = new MediaFile((char *) "/home/user/Videos/2020-02-12 16-11-32.flv");
        mf->initialise();
    }

    void VideoPlayer::initAL() {
        if (!AudioSystem::device && !InitAL(AudioSystem::device, AudioSystem::context, nullptr)) {
            std::cerr << "Failed to set up audio device" << std::endl;
            exit(-1);
        }
    }
}
