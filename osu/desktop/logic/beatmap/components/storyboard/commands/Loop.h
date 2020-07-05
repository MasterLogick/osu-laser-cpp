//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_LOOP_H
#define OSU_LASER_CPP_LOOP_H

#include "../CommandContainer.h"
#include "CompoundCommand.h"
#include <vector>
#include <string>

namespace osu {
    class Loop : public CompoundCommand {
    public:
        int loopCount;

        void commit() override;

        explicit Loop(std::vector<std::string> &s);
    };
}

#endif //OSU_LASER_CPP_LOOP_H
