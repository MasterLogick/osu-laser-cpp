//
// Created by MasterLogick on 6/9/20.
//

#include "Animation.h"
#include "../../../utill/StringUtills.h"
#include <vector>
#include <boost/lexical_cast.hpp>

namespace osu {
    Animation::Animation(std::string &line) {
        //0         1       2        3            4   5   6            7            8
        //Animation,(layer),(origin),”(filepath)”,(x),(y),(frameCount),(frameDelay),(looptype)
        std::vector<std::string> data = split(line, ",");
        if (data.size() != 9) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        layer = parseLayer(data[1]);
        origin = parseOrigin(data[2]);
        file = data[3].substr(data[3].front() == '"' ? 1 : 0,
                              data[3].size() - (data[3].front() == '"' ? 1 : 0) - (data[3].back() == '"' ? 1 : 0));
        pos.x = boost::lexical_cast<int>(data[4]);
        pos.y = boost::lexical_cast<int>(data[5]);
        frameCount = boost::lexical_cast<int>(data[6]);
        frameDelay = boost::lexical_cast<int>(data[7]);
        sswitch(data[8]) {
            scase("LoopForever"):
                type = LoopForever;
                break;
                scase("LoopOnce"):
                type = LoopOnce;
                break;
        }
    }
}