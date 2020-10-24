//
// Created by user on 18/10/2020.
//

#ifndef OSU_LASER_CPP_SLIDERPATH_H
#define OSU_LASER_CPP_SLIDERPATH_H

#include "../../../logic/utill/Point.h"
#include <cstdlib>
#include <vector>
#include <mutex>

namespace osu {
    class SliderPath {
        static Point *BUFFER;
        static std::mutex _t;
    public:
        std::vector<Point> mainSpline;

        SliderPath(Point *p, std::size_t size, float slideLength, float aprox);

        SliderPath();

    };
}

#endif //OSU_LASER_CPP_SLIDERPATH_H
