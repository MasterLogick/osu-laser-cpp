//
// Created by MasterLogick on 2/13/20.
//

#ifndef OSU_LASER_C_OSUDATABASE_H
#define OSU_LASER_C_OSUDATABASE_H

#include <cstdint>
#include <string>
#include "Database.h"

namespace osu {
    class OsuDatabase : Database {
        typedef std::pair<int, double> IDPair;
        struct TimingPoint {
            double BPM;
            double offset;
            bool inherited;
        };
        typedef int64_t DateTime;
    private:
        int version;
        int folderCount;
        bool accountUnlocked;
        DateTime unlockTime;
        std::string playerName;


    public:
        void load(const char *name) override;
    };
}

#endif //OSU_LASER_C_OSUDATABASE_H
