//
// Created by MasterLogick on 20/06/2020.
//

#include <boost/lexical_cast.hpp>
#include "Move.h"

namespace osu {

    Move::Move(std::vector<std::string> &s) : SimpleCommand(s) {
        //0 1        2           3         4         5         6       7
        //M,(easing),(starttime),(endtime),(start_x),(start_y),(end_x),(end_y)
        if (s.size() < 6) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        type = CommandType::CTMove;
        start.x = boost::lexical_cast<float>(s[4]);
        start.y = boost::lexical_cast<float>(s[5]);
        end.x = s.size() > 6 ? boost::lexical_cast<float>(s[6]) : start.x;
        end.y = s.size() > 7 ? boost::lexical_cast<float>(s[7]) : start.y;
    }

    void Move::process(int time) {
        //todo
    }
}