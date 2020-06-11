//
// Created by MasterLogick on 6/9/20.
//

#ifndef OSU_LASER_CPP_ORIGIN_H
#define OSU_LASER_CPP_ORIGIN_H

#include <string>

namespace osu {
    enum Origin {
        TopLeft = 0,
        Centre = 1,
        CentreLeft = 2,
        TopRight = 3,
        BottomCentre = 4,
        TopCentre = 5,
        Custom = 6,
        CentreRight = 7,
        BottomLeft = 8,
        BottomRight = 9
    };

    Origin parseOrigin(std::string &s);
}

#endif //OSU_LASER_CPP_ORIGIN_H
