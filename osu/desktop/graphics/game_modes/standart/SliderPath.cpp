//
// Created by user on 18/10/2020.
//

#include "SliderPath.h"
#include <cstring>
#include <cmath>

namespace osu {

    SliderPath::SliderPath(Point *p, std::size_t size, float slideLength, float aprox) {
        int controlPointCount = std::ceil(slideLength / aprox);
        mainSpline.reserve(controlPointCount);
        Point *buffer = new Point[size];
        for (int i = 0; i < controlPointCount; ++i) {
            memcpy(buffer, p, size * sizeof(Point));
            for (int j = size - 1; j > 0; --j) {
                for (int k = 0; k < j; ++k) {
                    buffer[k].interpolate((i + 1.0f) / controlPointCount, buffer[k + 1]);
                }
            }
            mainSpline.push_back(buffer[0]);

        }
        delete[] buffer;
    }

    SliderPath::SliderPath() {}
}