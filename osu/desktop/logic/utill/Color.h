//
// Created by MasterLogick on 4/26/20.
//

#ifndef OSU_LASER_C_COLOR_H
#define OSU_LASER_C_COLOR_H

#include <cstdint>

namespace osu {
    class Color {
    public:
        uint8_t red, green, blue, alpha;

        Color() : red(0), green(0), blue(0), alpha(255) {}

        Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : red(red), green(green), blue(blue),
                                                                         alpha(alpha) {}

        Color(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue), alpha(255) {}
    };
}

#endif //OSU_LASER_C_COLOR_H
