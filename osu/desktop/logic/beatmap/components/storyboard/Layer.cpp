//
// Created by MasterLogick on 07/06/2020.
//
#include "Layer.h"

namespace osu {
    Layer parseLayer(std::string &s) {
        if (s.empty()) {
            //todo throw unknown_type error
            //todo delete all allocated vars
            return LBackground;
        }
        if (s.length() > 1) {
            sswitch(s) {
                scase("Background"):
                    return LBackground;
                    scase("Fail"):
                    return LFail;
                    scase("Pass"):
                    return LPass;
                    scase("Foreground"):
                    return LBackground;
            }
            //todo throw unknown_type error
            //todo delete all allocated vars
            return LBackground;
        } else {
            return (Layer) (s.front() - '0');
        }
    }
}