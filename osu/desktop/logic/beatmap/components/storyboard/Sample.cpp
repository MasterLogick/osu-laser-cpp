//
// Created by MasterLogick on 6/9/20.
//

#include "Sample.h"
#include <boost/lexical_cast.hpp>

namespace osu {
    Sample::Sample(std::vector<std::string> &data) {
        //0      1    2     3          4
        //Sample,time,layer,"filepath",volume
        if (data.size() < 4) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        startTime = boost::lexical_cast<int>(data[1]);
        layer = parseLayer(data[2]);//todo catch exception
        file = data[3].substr(data[3].front() == '"' ? 1 : 0,
                              data[3].size() - (data[3].front() == '"' ? 1 : 0) - (data[3].back() == '"' ? 1 : 0));
        eventType = EventType::ETSample;
    }
}