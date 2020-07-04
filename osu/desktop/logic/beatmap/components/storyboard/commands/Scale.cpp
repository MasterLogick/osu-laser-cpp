//
// Created by MasterLogick on 20/06/2020.
//

#include <boost/lexical_cast.hpp>
#include "Scale.h"

namespace osu {
    Scale::Scale(std::vector<std::string> &s) : SimpleCommand(s) {
        //0 1        2           3         4             5
        //S,<easing>,<starttime>,<endtime>,<start_scale>,<end_scale>
        if (s.size() < 5) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        type = CommandType::CTScale;
        startScale = boost::lexical_cast<float>(s[4]);
        endScale = s.size() > 5 ? boost::lexical_cast<float>(s[5]) : startScale;
    }
}