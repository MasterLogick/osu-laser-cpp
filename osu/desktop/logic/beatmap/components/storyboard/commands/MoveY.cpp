//
// Created by MasterLogick on 20/06/2020.
//

#include <boost/lexical_cast.hpp>
#include "MoveY.h"

namespace osu {

    MoveY::MoveY(std::vector<std::string> &s) : SimpleCommand(s) {
        //0  1        2           3         4         5
        //MY,(easing),(starttime),(endtime),(start_x),(end_x)
        if (s.size() < 5) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        type = CommandType::CTMoveY;
        startY = boost::lexical_cast<float>(s[4]);
        endY = s.size() > 5 ? boost::lexical_cast<float>(s[5]) : startY;
    }
}