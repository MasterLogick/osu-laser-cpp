//
// Created by MasterLogick on 5/5/20.
//

#ifndef OSU_LASER_C_HITOBJECTPARSER_H
#define OSU_LASER_C_HITOBJECTPARSER_H

#include "../beatmap/components/BeatmapEnums.h"
#include "../beatmap/components/GameMode.h"
#include "HitObject.h"
#include <string>

namespace osu {
    class HitObjectParser {
    private:
        int formatVersion;
        int globalOffset;

    public:
        virtual HitObject *parseHitObject(std::string &line) = 0;

        explicit HitObjectParser(int version) : formatVersion(version), globalOffset(version < 5 ? 24 : 0) {}
    };
}

#endif //OSU_LASER_C_HITOBJECTPARSER_H
