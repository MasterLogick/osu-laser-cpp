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
            return Background;
        }
        if (s.length() > 1) {
            sswitch(s)
            {
                scase("Background"):
                return Background;
                scase("Video"):
                return Video;
                scase("Break"):
                return Break;
                scase("Colour"):
                return Colour;
                scase("Sprite"):
                return Sprite;
                scase("Sample"):
                return Sample;
                scase("Animation"):
                return Animation;
            }
            //todo throw unknown_type error
            //todo delete all allocated vars
            return Background;
        } else {
            return (EventType) (s.front() - '0');
        }
    }
}