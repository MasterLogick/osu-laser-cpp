//
// Created by MasterLogick on 4/27/20.
//

#ifndef OSU_LASER_C_COLORSCHEMA_H
#define OSU_LASER_C_COLORSCHEMA_H

#include <vector>
#include "../../utill/Color.h"

namespace osu {
    class ColorSchema {
    public:
        std::vector<Color> ComboColors;
        Color SliderTrackOverride;
        Color SliderBorder;

        //todo get colors from skin
        ColorSchema() : ComboColors(), SliderTrackOverride(0, 0, 0), SliderBorder(0, 0, 0) {}
    };
}

#endif //OSU_LASER_C_COLORSCHEMA_H
