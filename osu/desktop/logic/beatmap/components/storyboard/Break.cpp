//
// Created by MasterLogick on 28/06/2020.
//

#include "Break.h"
#include <boost/lexical_cast.hpp>

namespace osu {

    Break::Break(std::vector<std::string> &data) {
        //0 1         2
        //2,startTime,endTime
        if (data.size() != 3) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        startTime = boost::lexical_cast<int>(data[1]);
        endTime = boost::lexical_cast<int>(data[2]);
        eventType = EventType::ETBreak;
    }
}