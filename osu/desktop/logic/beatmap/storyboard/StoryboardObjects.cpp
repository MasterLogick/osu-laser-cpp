//
// Created by MasterLogick on 6/7/20.
//

#include "StoryboardObjects.h"

namespace osu {
    Sample::Sample(std::string &line) {
        //0      1    2     3          4
        //Sample,time,layer,"filepath",volume
        std::vector<std::string> data = split(line, ",");
        if (data.size() < 4) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        time = boost::lexical_cast<int>(data[1]);
        layer = parseLayer(data[2]);//todo catch exception
        file = std::move(data[3].substr(1, data[3].size() - 2));
    }
}