//
// Created by Masterlogick on 5/5/20.
//

#ifndef OSU_LASER_C_OSUHITOBJECT_H
#define OSU_LASER_C_OSUHITOBJECT_H

#include <cstdint>
#include "../HitSample.h"
#include "../../utill/Point.h"
#include "../HitObject.h"

namespace osu {

    class OsuHitObject : public HitObject {
    public:
        Point pos;
        uint8_t hitSoundBitField;
        HitSample hitSample;
    };

    class OsuCircle : public OsuHitObject {
    };

    enum CurveType {
        Bezier = 'B',
        Catmull = 'C',
        Linear = 'L',
        PerfectCircle = 'P'
    };

    class OsuSlider : public OsuHitObject {
    public:
        CurveType curveType;
        Point *curvePoints;
        std::size_t curvePointsCount;
        int slides;
        double length;
        std::size_t edgeCount;
        uint8_t *edgeSounds;
        HitSample *edgeSets;
    };

    class OsuSpinner : public OsuHitObject {
    public:
        int endTime;
    };
}
#endif //OSU_LASER_C_OSUHITOBJECT_H
