//
// Created by MasterLogick on 21/06/2020.
//

#include <boost/lexical_cast.hpp>
#include "Trigger.h"

namespace osu {

    Trigger::Trigger(std::vector<std::string> &s) {
        //0 1             2           3         4
        //T,(triggerType),(starttime),(endtime),(group_number)
        if (s.size() < 4) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        type = CommandType::CTTrigger;
        triggerType = TriggerType(s[1]);
        startTime = boost::lexical_cast<int>(s[2]);
        activeEndTime = boost::lexical_cast<int>(s[3]);
        groupNumber = s.size() == 4 ? 0 : boost::lexical_cast<int>(s[4]);
    }

    void Trigger::pack() {
        CompoundCommand::pack();
        endTime = activeEndTime + c.getEndTime();
    }

    void Trigger::process(int time) {
        //todo
    }
}