//
// Created by user on 20/10/2020.
//

#include "Point.h"

namespace osu {
    void Point::interpolate(float t, Point &b) {
        x += t * (b.x - x);
        y += t * (b.y - y);
    }
}