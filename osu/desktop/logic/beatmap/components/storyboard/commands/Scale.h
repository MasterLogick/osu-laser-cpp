//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_SCALE_H
#define OSU_LASER_CPP_SCALE_H

#include "SimpleCommand.h"

namespace osu {
    class Scale : public SimpleCommand {
    public:
        float startScale;
        float endScale;

        explicit Scale(std::vector<std::string> &s);

        void process(int time);
    };
}

#endif //OSU_LASER_CPP_SCALE_H
