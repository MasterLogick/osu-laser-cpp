//
// Created by MasterLogick on 20/06/2020.
//

#include <boost/lexical_cast.hpp>
#include "SimpleCommand.h"

namespace osu {
    SimpleCommand::SimpleCommand(std::vector<std::string> &s) {
        //0       1        2           3         4
        //(event),(easing),(starttime),(endtime),(params...)
        if (s.size() < 4) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        easing = parseEasing(s[1]);//todo catch exception
        startTime = boost::lexical_cast<int>(s[2]);
        endTime = s[3].empty() ? startTime : boost::lexical_cast<int>(s[3]);
    }

    SimpleCommand::SimpleCommand() {

    }
}