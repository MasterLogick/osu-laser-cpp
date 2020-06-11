//
// Created by MasterLogick on 4/25/20.
//

#ifndef OSU_LASER_C_TIMINGPOINTSET_H
#define OSU_LASER_C_TIMINGPOINTSET_H

#include <deque>
#include "TimingPoint.h"

namespace osu {
    class TimingPointSet {
    public:
        std::deque<TimingPoint> timingPointQueue;
    };
}

#endif //OSU_LASER_C_TIMINGPOINTSET_H
