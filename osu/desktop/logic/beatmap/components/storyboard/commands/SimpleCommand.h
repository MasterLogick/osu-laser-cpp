//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_SIMPLECOMMAND_H
#define OSU_LASER_CPP_SIMPLECOMMAND_H

#include "../../../../../graphics/UI/Easing.h"
#include "Command.h"
#include <vector>
#include <string>

namespace osu {
    class SimpleCommand : public Command {
    public:
        Easing easing;

        bool isCompound() override;

        explicit SimpleCommand(std::vector<std::string> &s);

        SimpleCommand();
    };
}

#endif //OSU_LASER_CPP_SIMPLECOMMAND_H