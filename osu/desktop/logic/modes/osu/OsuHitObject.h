//
// Created by Masterlogick on 5/5/20.
//

#ifndef OSU_LASER_C_OSUHITOBJECT_H
#define OSU_LASER_C_OSUHITOBJECT_H

#include <cstdint>
#include "../HitSample.h"
#include "../../utill/Point.h"

namespace osu {
    enum OsuHitObjectType {
        Circle = 1,
        Slider = 2,
        Spinner = 4
    };

    class OsuHitObject {
    public:
        Point pos{0, 0};
        int time{0};
        uint8_t hitSoundBitField{0};
        HitSample hitSample{};
    };

    class OsuCircle : public OsuHitObject {
    };

    enum CurveType {
        Bezier,
        Catmull,
        Linear,
        PerfectCircle
    };

    class OsuSlider : public OsuHitObject {
    public:
        CurveType curveType{Bezier};
        Point *curvePoints{nullptr};
        std::size_t curvePointsCount{0};
        int slides{1};
        double length;
        uint8_t *edgeSounds{nullptr};
        HitSample *edgeSets{nullptr};
        std::size_t edgeCount{0};
    };

    class OsuSpinner : public OsuHitObject {
    public:
        int endTime{-1};
    };
}
#endif //OSU_LASER_C_OSUHITOBJECT_H
