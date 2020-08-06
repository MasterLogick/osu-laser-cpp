//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_ROTATE_H
#define OSU_LASER_CPP_ROTATE_H

#include "SimpleCommand.h"

namespace osu {
    class Rotate : public SimpleCommand {
    public:
        float startAngle;
        float endAngle;

        explicit Rotate(std::vector<std::string> &s);

        void process(int time);
    };
}

#endif //OSU_LASER_CPP_ROTATE_H
