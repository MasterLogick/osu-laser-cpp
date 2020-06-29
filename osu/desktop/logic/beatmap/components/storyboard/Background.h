//
// Created by MasterLogick on 28/06/2020.
//

#ifndef OSU_LASER_CPP_BACKGROUND_H
#define OSU_LASER_CPP_BACKGROUND_H

#include "Event.h"
#include <vector>

namespace osu {
    class Background : public Event {
    public:
        int startTime;
        std::string path;
        int xoff;
        int yoff;

        explicit Background(std::vector<std::string> &data);
    };
}

#endif //OSU_LASER_CPP_BACKGROUND_H
