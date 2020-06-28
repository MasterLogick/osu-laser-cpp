//
// Created by MasterLogick on 21/06/2020.
//

#ifndef OSU_LASER_CPP_TRIGGERTYPE_H
#define OSU_LASER_CPP_TRIGGERTYPE_H

#include <string>

namespace osu {
    class TriggerType {
    private:
        unsigned int flag{0};
        int sampleNumber{0};
    public:
        explicit TriggerType(std::string &s);

        TriggerType();
    };
}

#endif //OSU_LASER_CPP_TRIGGERTYPE_H
