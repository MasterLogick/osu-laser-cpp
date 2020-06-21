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
            return Colour;
        }
        sswitch(s) {
            scase("C"):
                return Colour;
                scase("F"):
                return Fade;
                scase("L"):
                return Loop;
                scase("M"):
                return Move;
                scase("MX"):
                return MoveX;
                scase("MY"):
                return MoveY;
                scase("R"):
                return Rotate;
                scase("S"):
                return Scale;
                scase("T"):
                return Trigger;
                scase("V"):
                return VectorScale;

        }
        //todo throw unknown_type error
        //todo delete all allocated vars
        return Colour;
    }
}