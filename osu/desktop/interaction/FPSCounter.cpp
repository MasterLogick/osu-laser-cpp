//
// Created by MasterLogick on 2/5/20.
//
#include <thread>
#include "FPSCounter.h"
#include <ratio>

using namespace std::chrono;

namespace osu {

    FPSCounter::FPSCounter(int range) {
        _range = range;
        now = 0;
        passed = 0;
        durations = new double[range];
        start = future = past = system_clock::now();
    }

    FPSCounter::~FPSCounter() {
        delete[] durations;
    }

    double FPSCounter::getFPS() {
        return 1000000 * now / passed;
    }

    void FPSCounter::setRange(int range) {
        double *old = durations;
        _range = range;
        now = 0;
        passed = 0;
        durations = new double[range];
        delete[] old;
        start = future = past = system_clock::now();
    }

    void FPSCounter::countFPSAndSleep() {
        auto length = (future = system_clock::now()) - past;
        if (++now == _range) {
            now = 0;
            passed = 0;
        }
        durations[now] = duration_cast<microseconds>(length).count();
        past = future;
        if (future - start >= 1s) {
            start = future;
            now = 0;
            passed = 0;
            std::this_thread::sleep_until(start + microseconds(std::micro::den / _range));
        } else if (now == 0) {
            start += 1s;
            passed = 0;
            std::this_thread::sleep_until(start);
        } else {
            passed += durations[now];
            std::this_thread::sleep_until(start + microseconds(std::micro::den / _range * (now)));
        }
    }

    float FPSCounter::getDelta() {
        return durations[now];
    }
}