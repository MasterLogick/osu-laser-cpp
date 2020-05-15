//
// Created by Masterlogick on 5/8/20.
//

#ifndef OSU_LASER_C_HITSAMPLE_H
#define OSU_LASER_C_HITSAMPLE_H

#include <cstdint>
#include <string>

namespace osu {
    struct HitSample {
    public:
        uint8_t normalSet{0};
        uint8_t additionSet{0};
        int index{0};
        uint8_t volume{0};
        const char *filename{nullptr};
    };
}

#endif //OSU_LASER_C_HITSAMPLE_H
