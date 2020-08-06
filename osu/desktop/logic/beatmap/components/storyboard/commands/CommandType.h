//
// Created by MasterLogick on 21/06/2020.
//

#ifndef OSU_LASER_CPP_COMMANDTYPE_H
#define OSU_LASER_CPP_COMMANDTYPE_H

#include <string>

namespace osu {
    enum CommandType {
        CTColour, CTFade, CTLoop, CTMove, CTMoveX, CTMoveY, CTRotate, CTScale, CTTrigger, CTVectorScale, CTParameter,
        CTEventContainer //only event command container can have CTEventContainer CommandType
    };

    CommandType parseCommandType(std::string &s);
}

#endif //OSU_LASER_CPP_COMMANDTYPE_H
