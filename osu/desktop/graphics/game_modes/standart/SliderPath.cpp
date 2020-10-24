//
// Created by user on 18/10/2020.
//

#include "SliderPath.h"
#include <cstring>
#include <cmath>

#define BUFFER_SIZE 32
namespace osu {
    Point *SliderPath::BUFFER = new Point[BUFFER_SIZE];
    std::mutex SliderPath::_t = std::mutex();

    SliderPath::SliderPath(Point *p, std::size_t size, float slideLength, float aprox) {
        Point *buffer;
        if (size > BUFFER_SIZE) {
            buffer = new Point[size];
        } else {
            _t.lock();
            buffer = BUFFER;
        }
        memcpy(buffer, p, size * sizeof(Point));
        int controlPointCount = std::ceil(slideLength / aprox);
        mainSpline.reserve(controlPointCount);
        for (int i = 0; i < controlPointCount; ++i) {
            for (int j = size - 1; j > 0; --j) {
                for (int k = 0; k < j; ++k) {
                    buffer[i].interpolate((i + 1.0f) / controlPointCount, buffer[i + 1]);
                }
            }
            mainSpline.push_back(buffer[0]);
            memcpy(buffer, p, size * sizeof(Point));
        }
        if (size > BUFFER_SIZE) {
            delete[] buffer;
        } else {
            _t.unlock();
        }
    }

    SliderPath::SliderPath() {

    }
}