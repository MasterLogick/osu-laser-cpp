//
// Created by MasterLogick on 04/07/2020.
//

#ifndef OSU_LASER_CPP_PARAMETER_H
#define OSU_LASER_CPP_PARAMETER_H

#include "SimpleCommand.h"

namespace osu {
    class Parameter : public SimpleCommand {
    public:
        enum ParameterType {
            PTHorizontalFlip = 'H',
            PTVerticalFlip = 'V',
            PTAdditiveColourBlending = 'A'
        } ptype;

        explicit Parameter(std::vector<std::string> &s);

        void process(int time);
    };
}

#endif //OSU_LASER_CPP_PARAMETER_H
