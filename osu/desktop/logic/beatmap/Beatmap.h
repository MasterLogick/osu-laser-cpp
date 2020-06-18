//
// Created by MasterLogick on 4/3/20.
//

#ifndef OSU_LASER_C_BEATMAP_H
#define OSU_LASER_C_BEATMAP_H

#include "components/BeatmapMetadata.h"
#include "components/TimingPointSet.h"
#include "components/ColorSchema.h"
#include "../modes/HitObject.h"
#include <deque>

namespace osu {

    class Beatmap {
    public:
        BeatmapMetadata *metadata;
        TimingPointSet *timingPointSet;
        ColorSchema *colorSchema;
        std::deque<HitObject *> hitObjects;
    };
}

#endif //OSU_LASER_C_BEATMAP_H
