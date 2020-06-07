//
// Created by MasterLogick on 5/8/20.
//

#ifndef OSU_LASER_C_HITSAMPLE_H
#define OSU_LASER_C_HITSAMPLE_H

#include <cstdint>
#include <string>
#include <vector>
#include "../utill/StringUtills.h"
#include <boost/lexical_cast.hpp>

namespace osu {
    struct HitSample;

    struct HitSample {
    public:
        static HitSample DefaultHitsample;

        explicit HitSample() : normalSet(0), additionSet(0), index(0), volume(100), filename(nullptr) {

        }

        explicit HitSample(std::string sample) {
            std::vector<std::string> options = split(sample, ":");
            if (options[0].size() != 0)
                normalSet = boost::lexical_cast<int>(options[0]);
            else
                normalSet = 0;
            if (options[1].size() != 0)
                additionSet = boost::lexical_cast<int>(options[1]);
            else
                additionSet = 0;
            if (options.size() >= 3 && options[2].size() != 0)
                index = boost::lexical_cast<int>(options[2]);
            else
                index = 0;
            if (options.size() >= 4 && options[3].size() != 0)
                volume = boost::lexical_cast<int>(options[3]);
            else
                volume = 100;
            if (options.size() >= 5 && options[4].size() != 0)
                filename = options[4].c_str();
            else
                filename = nullptr;
        }

        uint8_t normalSet;
        uint8_t additionSet;
        int index;
        uint8_t volume;
        const char *filename;
    };
}

#endif //OSU_LASER_C_HITSAMPLE_H
