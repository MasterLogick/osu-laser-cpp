//
// Created by MasterLogick on 1/25/20.
//

#ifndef OSU_LASER_C_LOGIC_H
#define OSU_LASER_C_LOGIC_H

#include "beatmap/BeatmapLoader.h"

namespace osu {
    class Logic {
        static Beatmap *CurrentBeatmap;
    public:
        static void initialise();

        static void start();
    };
}

#endif //OSU_LASER_C_LOGIC_H
