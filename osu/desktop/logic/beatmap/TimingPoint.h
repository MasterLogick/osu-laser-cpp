//
// Created by Masterlogick on 4/25/20.
//

#ifndef OSU_LASER_C_TIMINGPOINT_H
#define OSU_LASER_C_TIMINGPOINT_H

#include "BeatmapEnums.h"

namespace osu {
    class TimingPoint {
    public:
        int time{0};
        double beatLength{0};
        double speedMultiplier{1};
        TimeSignatures timeSignature{Quadruple};
        SampleSets sampleSet;
        int sampleIndex{0};
        int sampleVolume{100};
        bool uninherited{true};
        bool kiaiMode{false};
        bool omitFirstBarSignature{false};

        static const int KIAI_MODE = 0x1;
        static const int OMIT_FIRST_BAR_SIGNATURE = 0x8;

        TimingPoint(SampleSets sampleSets) : sampleSet(sampleSets) {};
    };
}

#endif //OSU_LASER_C_TIMINGPOINT_H
