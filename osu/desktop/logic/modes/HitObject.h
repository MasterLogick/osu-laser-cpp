//
// Created by MasterLogick on 5/26/20.
//

#ifndef OSU_LASER_CPP_HITOBJECT_H
#define OSU_LASER_CPP_HITOBJECT_H

#include "../utill/Point.h"
#include "HitSample.h"
#include "../../graphics/game_modes/standart/SliderPath.h"

namespace osu {
    enum HitObjectType {
        HTCircle = 0b0000'0001,
        HTSlider = 0b0000'0010,
        HTSpinner = 0b0000'1000,
        HTHold = 0b1000'0000,
        HTNewCombo = 0b0000'0100,
        HTSkipColors = 0b0111'0000
    };
    struct Circle {
        int time;
        HitObjectType type;
        Point pos;
        uint8_t hitSoundBitField;
        HitSample hitSample;
        bool failed = false;
        int failTime;

    };

    enum CurveType {
        CTBezier = 'B',
        CTCatmull = 'C',
        CTLinear = 'L',
        CTPerfectCircle = 'P'
    };

    class Slider :
            public Circle {
    public:
        CurveType curveType;
        Point *curvePoints;
        std::size_t curvePointsCount;
        int slides;
        double length;
        std::size_t edgeCount;
        uint8_t *edgeSounds;
        HitSample *edgeSets;
        SliderPath *p;
    };

    class Spinner :
            public Circle {
    public:
        int endTime;
    };
}
#endif //OSU_LASER_CPP_HITOBJECT_H
