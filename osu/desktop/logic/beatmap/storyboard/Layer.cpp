//
// Created by MasterLogick on 07/06/2020.
//
#include "Layer.h"

namespace osu {
    Layer parseLayer(std::string s) {
        if (s.empty()) {
            //todo throw unknown_type error
            //todo delete all allocated vars
            return Background;
        }
        if (s.length() > 1) {
            sswitch(s) {
                scase("Background"):
                    return Background;
                    scase("Fail"):
                    return Fail;
                    scase("Pass"):
                    return Pass;
                    scase("Foreground"):
                    return Background;
            }
            //todo throw unknown_type error
            //todo delete all allocated vars
            return Background;
        } else {
            return (Layer) (s[0] - '0');
        }
    }
}