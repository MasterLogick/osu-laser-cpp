//
// Created by Masterlogick on 5/5/20.
//

#ifndef OSU_LASER_C_OSUHITOBJECTPARSER_H
#define OSU_LASER_C_OSUHITOBJECTPARSER_H

#include "../HitObjectParser.h"

namespace osu {
    class OsuHitObjectParser : HitObjectParser {
    private:
        int formatVersion;
        int globalOffset;
    public:
        OsuHitObjectParser(int version) : HitObjectParser(version), formatVersion(version),
                                          globalOffset(version < 5 ? 24 : 0) {}

        HitObject * parseHitObject(std::string line) override;
    };
}

#endif //OSU_LASER_C_OSUHITOBJECTPARSER_H
