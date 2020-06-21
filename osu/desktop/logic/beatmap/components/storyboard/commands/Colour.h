//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_COLOUR_H
#define OSU_LASER_CPP_COLOUR_H

#include "SimpleCommand.h"
#include "../../../../utill/Color.h"

namespace osu {
    class Colour : public SimpleCommand {
    public:
        Color start;
        Color end;

        explicit Colour(std::vector<std::string> &s);
    };
}

#endif //OSU_LASER_CPP_COLOUR_H
