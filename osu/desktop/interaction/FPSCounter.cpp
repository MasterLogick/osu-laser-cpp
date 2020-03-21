//
// Created by Masterlogick on 2/5/20.
//
#include <thread>
#include "FPSCounter.h"

namespace osu {

    FPSCounter::FPSCounter(int range) {
        _range = range;
        now = 0;
        durations = new double[range];
        future = past = high_resolution_clock::now();
    }

    FPSCounter::~FPSCounter() {
        delete[] durations;
    }

    double FPSCounter::getFPS() {
        return 1000 / durations[now];
    }

    void FPSCounter::setRange(int range) {
        double *old = durations;
        _range = range;
        now = 0;
        durations = new double[range];
        past = high_resolution_clock::now();
        future = high_resolution_clock::now();
        delete[] old;
    }

    void FPSCounter::countFPSAndSleep() {
        auto length = (future = high_resolution_clock::now()) - past;
        if (++now == _range) {
            now = 0;
        }
        durations[now] = length.count();
        past = future;
        std::this_thread::sleep_until(future + microseconds(1000000 / _range * 2) - length);
    }

    float FPSCounter::getDelta() {
        return durations[now];
    }
}