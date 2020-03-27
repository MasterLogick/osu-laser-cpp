//
// Created by Masterlogick on 2/5/20.
//
#include <thread>
#include "FPSCounter.h"

using namespace std::chrono;

namespace osu {

    FPSCounter::FPSCounter(int range) {
        _range = range;
        now = 0;
        durations = new double[range];
        start = future = past = high_resolution_clock::now();
    }

    FPSCounter::~FPSCounter() {
        delete[] durations;
    }

    double FPSCounter::getFPS() {
        return 1000000 / durations[now];
    }

    void FPSCounter::setRange(int range) {
        double *old = durations;
        _range = range;
        now = 0;
        frames = 0;
        durations = new double[range];
        delete[] old;
        start = future = past = high_resolution_clock::now();
    }

    void FPSCounter::countFPSAndSleep() {
        auto length = (future = high_resolution_clock::now()) - past;
        if (++now == _range) {
            now = 0;
        }
        durations[now] = duration_cast<microseconds>(length).count();
        past = future;
        frames++;
        if (future - start >= 1s) {
            start = future;
            frames = 0;
            std::this_thread::sleep_until(start + microseconds(1000000 / _range));
        } else if (frames == _range) {
            start += 1s;
            frames = 0;
            std::this_thread::sleep_until(start);
        } else {
            std::this_thread::sleep_until(start + microseconds(1000000 / _range * (frames)));
        }
    }

    float FPSCounter::getDelta() {
        return durations[now];
    }
}