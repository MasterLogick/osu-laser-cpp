//
// Created by MasterLogick on 28/06/2020.
//

#include "EvVideo.h"
#include <boost/lexical_cast.hpp>

namespace osu {
    EvVideo::EvVideo(std::vector<std::string> &data) {
        //0     1         2        3       4
        //Video,startTime,filename,xOffset,yOffset
        if (data.size() < 3) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        startTime = boost::lexical_cast<int>(data[1]);
        if (data.size() == 5) {
            xoff = boost::lexical_cast<int>(data[3]);
            yoff = boost::lexical_cast<int>(data[4]);
        }
        eventType = EventType::ETVideo;
    }
}