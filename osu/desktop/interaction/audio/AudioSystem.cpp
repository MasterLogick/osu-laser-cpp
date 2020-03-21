//
// Created by MasterLogick on 1/25/20.
//
#include <iostream>
#include "al.h"
#include "alc.h"
#include "common/alhelpers.h"

#include "AudioSystem.h"
#include "../video/MediaFile.h"

namespace osu {
    MediaFile *AudioSystem::mf = nullptr;
    ALCdevice *AudioSystem::device = nullptr;
    ALCcontext *AudioSystem::context = nullptr;
    LPALGETSOURCEI64VSOFT  AudioSystem::alGetSourcei64vSOFT = nullptr;
    LPALCGETINTEGER64VSOFT AudioSystem::alcGetInteger64vSOFT = nullptr;

    void AudioSystem::initialise() {
        if (!InitAL(AudioSystem::device, AudioSystem::context, nullptr)) {
            std::cerr << "Failed to set up audio device" << std::endl;
            exit(-1);
        }
        if (alcIsExtensionPresent(AudioSystem::device, "ALC_SOFT_device_clock")) {
            alcGetInteger64vSOFT = reinterpret_cast<LPALCGETINTEGER64VSOFT>(
                    alcGetProcAddress(AudioSystem::device, "alcGetInteger64vSOFT")
            );
        }
        if (alIsExtensionPresent("AL_SOFT_source_latency")) {
            alGetSourcei64vSOFT = reinterpret_cast<LPALGETSOURCEI64VSOFT>(
                    alGetProcAddress("alGetSourcei64vSOFT")
            );
        }
        loadSounds();
//        MediaFile("/home/user/CLionProjects/osu-laser-cpp/osu-resources/osu.Game.Resources/Textures/Menu/logo-triangles.mp4");
//        MediaFile("/home/user/CLionProjects/osu-laser-cpp/osu-resources/osu.Game.Resources/Textures/Menu/logo-triangles.mp4");
//        MediaFile("/home/user/CLionProjects/osu-laser-cpp/osu-resources/osu.Game.Resources/Samples/seeya.mp3");
        AudioSystem::mf = new MediaFile("/home/user/Videos/2020-01-09 17-27-55.flv");
    }

    void AudioSystem::start() {

    }

    void AudioSystem::loadSounds() {

    }
}
