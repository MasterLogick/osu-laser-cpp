//
// Created by MasterLogick on 21/06/2020.
//

#ifndef OSU_LASER_CPP_TRIGGER_H
#define OSU_LASER_CPP_TRIGGER_H

#include "CompoundCommand.h"
#include "TriggerType.h"
#include <vector>
#include <string>

namespace osu {
    class Trigger : public CompoundCommand {
    public:
        TriggerType triggerType;
        int groupNumber;
        int activeEndTime;

        explicit Trigger(std::vector<std::string> &s);
    };
}

#endif //OSU_LASER_CPP_TRIGGER_H
