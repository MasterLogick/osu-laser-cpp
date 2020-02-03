//
// Created by user on 1/25/20.
//

#ifndef OSU_LASER_C_MSAASYSTEM_H
#define OSU_LASER_C_MSAASYSTEM_H

namespace osu {
    class MSAASystem {
    public:
        explicit MSAASystem(int msaaLevel);

        void prepareForDraw();

        void render(int destinationFramebuffer);
    };
}

#endif //OSU_LASER_C_MSAASYSTEM_H
