//
// Created by MasterLogick on 20/06/2020.
//

#include "EventType.h"
#include "../../../utill/str_switch.h"

namespace osu {
    EventType parseEventType(std::string &s) {
        if (s.empty()) {
            //todo throw unknown_type error
            //todo delete all allocated vars
            return ETBackground;
        }
        if (s.length() > 1) {
            sswitch(s)
            {
                scase("Background"):
                    return ETBackground;
                scase("Video"):
                    return ETVideo;
                scase("Break"):
                    return ETBreak;
                scase("Colour"):
                    return ETColour;
                scase("Sprite"):
                    return ETSprite;
                scase("Sample"):
                    return ETSample;
                scase("Animation"):
                    return ETAnimation;
            }
            //todo throw unknown_type error
            //todo delete all allocated vars
            return ETBackground;
        } else {
            return (EventType) (s.front() - '0');
        }
    }
}