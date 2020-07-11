//
// Created by MasterLogick on 1/25/20.
//

#ifndef OSU_LASER_C_AUDIOSYSTEM_H
#define OSU_LASER_C_AUDIOSYSTEM_H

#include "alext.h"
#include "alc.h"

namespace osu {
    class AudioSystem {
    private:
        static void loadSounds();

    public:
        static LPALGETSOURCEI64VSOFT alGetSourcei64vSOFT;
        static LPALCGETINTEGER64VSOFT alcGetInteger64vSOFT;
        static ALCdevice *device;

        static ALCcontext *context;

        static void initialise();

        static void start();
    };
}
//}
#endif //OSU_LASER_C_AUDIOSYSTEM_H
