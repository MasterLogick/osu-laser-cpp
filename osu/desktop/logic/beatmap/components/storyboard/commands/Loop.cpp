//
// Created by MasterLogick on 20/06/2020.
//

#include <boost/lexical_cast.hpp>
#include "Loop.h"

namespace osu {

    Loop::Loop(std::vector<std::string> &s) : c() {
        //0 1           2
        //L,(starttime),(loopcount)
        if (s.size() < 3) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        type = CommandType::Loop;
        startTime = boost::lexical_cast<int>(s[1]);
        loopCount = boost::lexical_cast<int>(s[2]);
    }

    int Loop::calcEndTime() {
        return 0;//todo implement
    }
}