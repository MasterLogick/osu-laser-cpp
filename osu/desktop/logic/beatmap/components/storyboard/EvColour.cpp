//
// Created by MasterLogick on 10/14/20.
//

#include <boost/lexical_cast.hpp>
#include "EvColour.h"

namespace osu {
    EvColour::EvColour(std::vector<std::string> &data) {
        //0 1         2 3 4
        //3,startTime,r,g,b
        if (data.size() != 3) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        startTime = boost::lexical_cast<int>(data[1]);
        r = boost::lexical_cast<int>(data[2]);
        g = boost::lexical_cast<int>(data[3]);
        b = boost::lexical_cast<int>(data[4]);
        eventType = EventType::ETColour;
    }
}