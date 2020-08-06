//
// Created by MasterLogick on 20/06/2020.
//

#include <boost/lexical_cast.hpp>
#include "Colour.h"

namespace osu {

    Colour::Colour(std::vector<std::string> &s) : SimpleCommand(s) {
        //0 1        2           3         4         5         6         7       8       9
        //C,(easing),(starttime),(endtime),(start_r),(start_g),(start_b),(end_r),(end_g),(end_b)
        if (s.size() < 7) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        type = CommandType::CTColour;
        start.red = boost::lexical_cast<int>(s[4]);
        start.green = boost::lexical_cast<int>(s[5]);
        start.blue = boost::lexical_cast<int>(s[6]);
        end.red = s.size() > 7 ? boost::lexical_cast<float>(s[7]) : start.red;
        end.green = s.size() > 8 ? boost::lexical_cast<float>(s[8]) : start.green;
        end.blue = s.size() > 9 ? boost::lexical_cast<float>(s[9]) : start.blue;
    }

    void Colour::process(int time) {
        //todo
    }
}