//
// Created by MasterLogick on 1/25/20.
//
#include <iostream>
#include "al.h"
#include "alc.h"
#include "common/alhelpers.h"

#include "AudioSystem.h"

namespace osu {
    ALCdevice *AudioSystem::device = nullptr;
    ALCcontext *AudioSystem::context = nullptr;
    LPALGETSOURCEI64VSOFT  AudioSystem::alGetSourcei64vSOFT = nullptr;
    LPALCGETINTEGER64VSOFT AudioSystem::alcGetInteger64vSOFT = nullptr;
    bool AudioSystem::initialised = false;

    void AudioSystem::initialise() {
        if (initialised)return;
        if (!device && !InitAL(device, context, nullptr)) {
            std::cerr << "Failed to set up audio device" << std::endl;
            exit(-1);
        }
        if (alcIsExtensionPresent(device, "ALC_SOFT_device_clock")) {
            alcGetInteger64vSOFT = reinterpret_cast<LPALCGETINTEGER64VSOFT>(
                    alcGetProcAddress(device, "alcGetInteger64vSOFT")
            );
        }
        if (alIsExtensionPresent("AL_SOFT_source_latency")) {
            alGetSourcei64vSOFT = reinterpret_cast<LPALGETSOURCEI64VSOFT>(
                    alGetProcAddress("alGetSourcei64vSOFT")
            );
        }
        loadSounds();
        initialised = true;
    }

    void AudioSystem::loadSounds() {

    }

    bool AudioSystem::isInitialised() {
        return initialised;
    }
}
