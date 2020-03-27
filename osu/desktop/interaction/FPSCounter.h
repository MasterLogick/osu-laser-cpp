//
// Created by MasterLogick on 1/25/20.
//

#ifndef OSU_LASER_C_FPSCOUNTER_H
#define OSU_LASER_C_FPSCOUNTER_H

#include <chrono>
#include <iostream>

using namespace std::chrono;
namespace osu {

    class FPSCounter {
    private:
        time_point<system_clock> start;
        time_point<system_clock> past;
        time_point<system_clock> future;
        int _range;
        int now;
        int frames = 0;
        double *durations;
    public:
        explicit FPSCounter(int range);

        ~FPSCounter();

        double getFPS();

        void setRange(int range);

        void countFPSAndSleep();

        float getDelta();
    };
}

#endif //OSU_LASER_C_FPSCOUNTER_H
