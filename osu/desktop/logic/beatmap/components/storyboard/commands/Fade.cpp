//
// Created by MasterLogick on 20/06/2020.
//

#include <boost/lexical_cast.hpp>
#include "Fade.h"

namespace osu {
    Fade::Fade(std::vector<std::string> &s) : SimpleCommand(s) {
        //0 1        2           3         4               5
        //F,(easing),(starttime),(endtime),(start_opacity),(end_opacity)
        if (s.size() < 5) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        type = CommandType::CTFade;
        startOpacity = boost::lexical_cast<float>(s[4]);
        endOpacity = s.size() > 5 ? boost::lexical_cast<float>(s[5]) : startOpacity;
    }

    void Fade::process(int time) {
        //todo
    }
}