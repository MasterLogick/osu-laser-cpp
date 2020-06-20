//
// Created by MasterLogick on 20/06/2020.
//

#include "Easing.h"

namespace osu {
    Easing parseEasing(std::string &s) {
        if (s.empty()) {
            //todo throw unknown_type error
            //todo delete all allocated vars
            return Linear;
        }
        if (s.length() > 2) {
            //todo throw unknown_type error
            //todo delete all allocated vars
            return Linear;
        } else {
            return (Easing) (strtol(s.c_str(), nullptr, 10));
            //todo catch exceptions using errno
        }
    }
}