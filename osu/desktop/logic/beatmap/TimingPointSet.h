//
// Created by Masterlogick on 4/25/20.
//

#ifndef OSU_LASER_C_TIMINGPOINTSET_H
#define OSU_LASER_C_TIMINGPOINTSET_H

#include <list>
#include "TimingPoint.h"

namespace osu {
    class TimingPointSet {
    public:
        std::list<TimingPoint> timingPointQueue;
    };
}

#endif //OSU_LASER_C_TIMINGPOINTSET_H
