//
// Created by MasterLogick on 6/9/20.
//

#include "Origin.h"
#include "../../logic/utill/str_switch.h"

namespace osu {
    Origin parseOrigin(std::string &s) {
        if (s.empty()) {
            //todo throw unknown_type error
            //todo delete all allocated vars
            return TopLeft;
        }
        if (s.length() > 1) {
            sswitch(s) {
                scase("TopLeft"):
                    return TopLeft;
                    scase("Centre"):
                    return Centre;
                    scase("CentreLeft"):
                    return CentreLeft;
                    scase("TopRight"):
                    return TopRight;
                    scase("BottomCentre"):
                    return BottomCentre;
                    scase("TopCentre"):
                    return TopCentre;
                    scase("Custom"):
                    return Custom;
                    scase("CentreRight"):
                    return CentreRight;
                    scase("BottomLeft"):
                    return BottomLeft;
                    scase("BottomRight"):
                    return BottomRight;
            }
            //todo throw unknown_type error
            //todo delete all allocated vars
            return TopLeft;
        } else {
            return (Origin) (s.front() - '0');
        }
    }
}