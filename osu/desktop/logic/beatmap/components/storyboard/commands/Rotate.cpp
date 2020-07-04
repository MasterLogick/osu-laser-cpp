//
// Created by MasterLogick on 20/06/2020.
//

#include <boost/lexical_cast.hpp>
#include "Rotate.h"

namespace osu {

    Rotate::Rotate(std::vector<std::string> &s) : SimpleCommand(s) {
        //0 1        2           3         4               5
        //R,<easing>,<starttime>,<endtime>,<start_rotate>,<end_rotate>
        if (s.size() < 5) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        type = CommandType::CTRotate;
        startAngle = boost::lexical_cast<float>(s[4]);
        endAngle = s.size() > 5 ? boost::lexical_cast<int>(s[5]) : startAngle;
    }
}