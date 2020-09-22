//
// Created by MasterLogick on 9/11/20.
//

#ifndef OSU_LASER_CPP_EXECUTIONTIMER_H
#define OSU_LASER_CPP_EXECUTIONTIMER_H

#include <chrono>
#include <iostream>

class ExecutionTimer {
    std::chrono::system_clock::time_point start;
public:
    ExecutionTimer() {
        start = std::chrono::system_clock::now();
    }

    ~ExecutionTimer() {
        std::chrono::duration e = std::chrono::system_clock::now() - start;
        std::chrono::duration secs = std::chrono::duration_cast<std::chrono::seconds>(e);
        e = e - secs;
        std::chrono::duration millisecs = std::chrono::duration_cast<std::chrono::milliseconds>(e);
        e = e - millisecs;
        std::chrono::duration microsecs = std::chrono::duration_cast<std::chrono::microseconds>(e);
        e = e - microsecs;
        std::chrono::duration nanosecs = std::chrono::duration_cast<std::chrono::nanoseconds>(e);
        std::cout << "Execution time: " << secs.count() << "s " << millisecs.count() << "ms " << microsecs.count()
                  << "us " << nanosecs.count() << "ns" << std::endl;
    }
};


#endif //OSU_LASER_CPP_EXECUTIONTIMER_H
