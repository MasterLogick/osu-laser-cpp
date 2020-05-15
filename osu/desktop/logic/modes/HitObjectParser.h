//
// Created by Masterlogick on 5/5/20.
//

#ifndef OSU_LASER_C_HITOBJECTPARSER_H
#define OSU_LASER_C_HITOBJECTPARSER_H

#include "../beatmap/BeatmapEnums.h"
#include "../beatmap/GameMode.h"
#include <string>

namespace osu {
    class HitObjectParser {
    private:
        int formatVersion;
        int globalOffset;

    public:
        virtual void *parseHitObject(std::string line) = 0;

        explicit HitObjectParser(int version) : formatVersion(version), globalOffset(version < 5 ? 24 : 0) {}

        static HitObjectParser *getParserByMode(GameMode mode) {
            switch (mode) {
                case Standard:
                    //todo return standard parser
                    break;
                case Taiko:
                    //todo return taiko parser
                    break;
                case Mania:
                    //todo return mania parser
                    break;
                case Catch:
                    //todo return catch parser
                    break;
            }
        }
    };
}

#endif //OSU_LASER_C_HITOBJECTPARSER_H
