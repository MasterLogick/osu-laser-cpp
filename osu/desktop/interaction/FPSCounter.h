//
// Created by user on 1/25/20.
//

#ifndef OSU_LASER_C_FPSCOUNTER_H
#define OSU_LASER_C_FPSCOUNTER_H

#include <chrono>
#include <iostream>

using namespace std::chrono;
namespace osu {

    class FPSCounter : duration<double, std::milli> {
    private:
        time_point<system_clock, duration<double, std::milli>> past;
        time_point<system_clock, duration<double, std::milli>> future;
        int _range;
        int now;
        double *durations;
    public:
        explicit FPSCounter(int range) {
            _range = range;
            now = 0;
            durations = new double[range];
            past = high_resolution_clock::now();
            future = high_resolution_clock::now();
        }

        ~FPSCounter() {
            delete[] durations;
        }

        double getFPS() {
            return 1000 / durations[now];
        }

        void setRange(int range) {
            double *old = durations;
            _range = range;
            now = 0;
            durations = new double[range];
//            perPeriod = 0;
            past = high_resolution_clock::now();
            future = high_resolution_clock::now();
            delete[] old;
        }

        void countFPSAndSleep() {
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

        float getDelta() {
            return durations[now];
        }
    };
}

#endif //OSU_LASER_C_FPSCOUNTER_H
