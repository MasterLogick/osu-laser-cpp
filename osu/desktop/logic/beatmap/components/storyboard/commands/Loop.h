//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_LOOP_H
#define OSU_LASER_CPP_LOOP_H

#include "Command.h"
#include "../CommandsContainer.h"
#include <vector>
#include <string>

namespace osu {
    class Loop : public Command {
    private:
        CommandsContainer c;
    public:
        int loopCount;
        int endTime;

        explicit Loop(std::vector<std::string> &s);

        int calcEndTime();
    };
}

#endif //OSU_LASER_CPP_LOOP_H
