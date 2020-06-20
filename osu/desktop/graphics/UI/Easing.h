//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_EASING_H
#define OSU_LASER_CPP_EASING_H

#include <string>

namespace osu {
    enum Easing {
        Linear = 0,
        Easing_Out = 1,
        Easing_In = 2,
        Quad_In = 3,
        Quad_Out = 4,
        Quad_In_Out = 5,
        Cubic_In = 6,
        Cubic_Out = 7,
        Cubic_In_Out = 8,
        Quart_In = 9,
        Quart_Out = 10,
        Quart_In_Out = 11,
        Quint_In = 12,
        Quint_Out = 13,
        Quint_In_Out = 14,
        Sine_In = 15,
        Sine_Out = 16,
        Sine_In_Out = 17,
        Expo_In = 18,
        Expo_Out = 19,
        Expo_In_Out = 20,
        Circ_In = 21,
        Circ_Out = 22,
        Circ_In_Out = 23,
        Elastic_In = 24,
        Elastic_Out = 25,
        ElasticHalf_Out = 26,
        ElasticQuarter_Out = 27,
        Elastic_In_Out = 28,
        Back_In = 29,
        Back_Out = 30,
        Back_In_Out = 31,
        Bounce_In = 32,
        Bounce_Out = 33,
        Bounce_In_Out = 34,
    };

    Easing parseEasing(std::string &s);
}

#endif //OSU_LASER_CPP_EASING_H
