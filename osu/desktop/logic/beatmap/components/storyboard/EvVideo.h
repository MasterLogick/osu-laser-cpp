//
// Created by MasterLogick on 28/06/2020.
//

#ifndef OSU_LASER_CPP_EVVIDEO_H
#define OSU_LASER_CPP_EVVIDEO_H

#include "Event.h"
#include <vector>

namespace osu {
    class EvVideo : public Event {
    public:
        std::string path;
        int xoff;
        int yoff;

        explicit EvVideo(std::vector<std::string> &data);
    };
}

#endif //OSU_LASER_CPP_EVVIDEO_H
