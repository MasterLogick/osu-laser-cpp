//
// Created by MasterLogick on 2/13/20.
//

#ifndef OSU_LASER_C_DATABASE_H
#define OSU_LASER_C_DATABASE_H

#include <utility>

namespace osu {


    class Database {
    public:
        virtual void load(const char *name) = 0;
    };
}
#endif //OSU_LASER_C_DATABASE_H
