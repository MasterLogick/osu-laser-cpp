//
// Created by MasterLogick on 1/25/20.
//

#ifndef OSU_LASER_C_TRIANGLEBACKGROUND_H
#define OSU_LASER_C_TRIANGLEBACKGROUND_H

#include "Component.h"
#include <random>

namespace osu {
    class TriangleBackground : public Component {
    private:
        unsigned int vao;
        unsigned int localPosVBO;
        unsigned int globalPosVBO;
        unsigned int colorVBO;
        unsigned int scaleVBO;
        int amount;
        float *positions;
        float *scale;
        float *color;
        float *velocity;
        std::mt19937 rndDevice;

        void movePositions();

    public:

        explicit TriangleBackground(int trianglesCount);

        void initialise();

        void draw(int x, int y) override;
    };
}

#endif //OSU_LASER_C_TRIANGLEBACKGROUND_H
