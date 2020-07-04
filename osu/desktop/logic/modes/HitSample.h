//
// Created by MasterLogick on 5/8/20.
//

#ifndef OSU_LASER_C_HITSAMPLE_H
#define OSU_LASER_C_HITSAMPLE_H

#include <cstdint>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

namespace osu {
    struct HitSample;

    struct HitSample {
    public:
        uint8_t normalSet;
        uint8_t additionSet;
        int index;
        uint8_t volume;
        const char *filename;

        static HitSample DefaultHitsample;

        explicit HitSample() : normalSet(0), additionSet(0), index(0), volume(100), filename(nullptr) {}

        explicit HitSample(std::string sample);


    };
}

#endif //OSU_LASER_C_HITSAMPLE_H
