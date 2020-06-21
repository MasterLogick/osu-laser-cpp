//
// Created by MasterLogick on 21/06/2020.
//

#ifndef OSU_LASER_CPP_TRIGGER_H
#define OSU_LASER_CPP_TRIGGER_H

#include "Command.h"
#include "TriggerType.h"
#include "../CommandsContainer.h"
#include <vector>
#include <string>

namespace osu {
    class Trigger : public Command {
    private:
        CommandsContainer c;
    public:
        int endTime;
        TriggerType triggerType;
        int groupNumber;

        explicit Trigger(std::vector<std::string> &s);
    };
}

#endif //OSU_LASER_CPP_TRIGGER_H
