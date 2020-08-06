//
// Created by MasterLogick on 20/06/2020.
//

#ifndef OSU_LASER_CPP_FADE_H
#define OSU_LASER_CPP_FADE_H

#include "SimpleCommand.h"

namespace osu {
    class Fade : public SimpleCommand {
    public:
        float startOpacity;
        float endOpacity;

        explicit Fade(std::vector<std::string> &s);

        void process(int time);
    };
}

#endif //OSU_LASER_CPP_FADE_H
