//
// Created by MasterLogick on 21/06/2020.
//

#ifndef OSU_LASER_CPP_COMMANDTYPE_H
#define OSU_LASER_CPP_COMMANDTYPE_H

#include <string>

namespace osu {
    enum CommandType {
        Colour, Fade, Loop, Move, MoveX, MoveY, Rotate, Scale, Trigger, VectorScale
    };

    CommandType parseCommandType(std::string &s);
}

#endif //OSU_LASER_CPP_COMMANDTYPE_H
