//
// Created by MasterLogick on 21/06/2020.
//

#include "CommandType.h"
#include "../../../../utill/str_switch.h"

namespace osu {
    CommandType parseCommandType(std::string &s) {
        if (s.size() > 2) {
            //todo throw unknown_type error
            //todo delete all allocated vars
            return CTColour;
        }
        sswitch(s) {
            scase("C"):
                return CTColour;
                scase("F"):
                return CTFade;
                scase("L"):
                return CTLoop;
                scase("M"):
                return CTMove;
                scase("MX"):
                return CTMoveX;
                scase("MY"):
                return CTMoveY;
                scase("R"):
                return CTRotate;
                scase("S"):
                return CTScale;
                scase("T"):
                return CTTrigger;
                scase("V"):
                return CTVectorScale;

        }
        //todo throw unknown_type error
        //todo delete all allocated vars
        return CTColour;
    }
}