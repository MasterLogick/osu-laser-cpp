//
// Created by MasterLogick on 9/11/20.
//

#ifndef OSU_LASER_CPP_DRAWABLEEVENT_H
#define OSU_LASER_CPP_DRAWABLEEVENT_H

#include "Event.h"
#include "../../../../graphics/UI/Texture.h"

namespace osu {
    class DrawableEvent : public Event {
    public:
        Texture *texture;
    };
}
#endif //OSU_LASER_CPP_DRAWABLEEVENT_H
