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
        past = high_resolution_clock::now();
        future = high_resolution_clock::now();
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
//            perPeriod = 0;
        past = high_resolution_clock::now();
        future = high_resolution_clock::now();
        delete[] old;
    }

    void FPSCounter::countFPSAndSleep() {
        duration<double, std::milli> length =
                (future = high_resolution_clock::now()) - past;
        if (++now == _range) {
            now = 0;
        }
        durations[now] = length.count();
        past = future;
//            struct tm *timer_info;
//            char buff[26];
//            time_t a = system_clock::to_time_t(system_clock::now());
//            timer_info = localtime(&a);
//            strftime(buff, 26, "%Y-%m-%d %H:%M:%S", timer_info);
//            std::cout << buff << " " << getFPS() << " " << durations[now] << " " << now << std::endl;
        std::this_thread::sleep_until(
                time_point_cast<seconds>(future) + microseconds(1000000 * (now + 1) / _range));
    }

    float FPSCounter::getDelta() {
        return durations[now];
    }
}