//
// Created by MasterLogick on 21/06/2020.
//

#include <boost/lexical_cast.hpp>
#include "TriggerType.h"
#include "../../../../utill/StringUtills.h"

#define PASSING 0
#define FAILING 1<<0
#define HITSOUND 1<<1
#define SS_ALL 1<<2
#define SS_NORMAL 1<<3
#define SS_SOFT 1<<4
#define SS_DRUM 1<<5
#define A_SS_ALL 1<<6
#define A_SS_NORMAL 1<<7
#define A_SS_SOFT 1<<8
#define A_SS_DRUM 1<<9
#define ADD_ALL 1<<10
#define ADD_WHISTLE 1<<11
#define ADD_FINISH 1<<12
#define ADD_CLAP 1<<13
namespace osu {

    TriggerType::TriggerType() = default;

    TriggerType::TriggerType(std::string &s) {
        const char *s1 = s.c_str();
        if (s.compare("Passing")) {
            flag = PASSING;
            return;
        }
        if (s.compare("Failing")) {
            flag = FAILING;
            return;
        }
        if (startsWith(s, "HitSound")) {
            flag = HITSOUND;
            std::string modifiers = s.substr(8);
            if (startsWith(modifiers, "All")) {
                flag |= SS_ALL;
                modifiers = modifiers.substr(3);
            } else if (startsWith(modifiers, "Drum")) {
                flag |= SS_DRUM;
                modifiers = modifiers.substr(4);
            } else if (startsWith(modifiers, "Normal")) {
                flag |= SS_NORMAL;
                modifiers = modifiers.substr(6);
            } else if (startsWith(modifiers, "Soft")) {
                flag |= SS_SOFT;
                modifiers = modifiers.substr(4);
            } else {
                flag |= SS_ALL;
            }
            if (startsWith(modifiers, "All")) {
                flag |= A_SS_ALL;
                modifiers = modifiers.substr(3);
            } else if (startsWith(modifiers, "Drum")) {
                flag |= A_SS_DRUM;
                modifiers = modifiers.substr(4);
            } else if (startsWith(modifiers, "Normal")) {
                flag |= A_SS_NORMAL;
                modifiers = modifiers.substr(6);
            } else if (startsWith(modifiers, "Soft")) {
                flag |= A_SS_SOFT;
                modifiers = modifiers.substr(4);
            } else {
                flag |= A_SS_ALL;
            }
            if (startsWith(modifiers, "Whistle")) {
                flag |= ADD_WHISTLE;
                modifiers = modifiers.substr(7);
            } else if (startsWith(modifiers, "Clap")) {
                flag |= ADD_CLAP;
                modifiers = modifiers.substr(4);
            } else if (startsWith(modifiers, "Finish")) {
                flag |= ADD_FINISH;
                modifiers = modifiers.substr(6);
            } else {
                flag |= ADD_ALL;
            }
            if (!modifiers.empty()) {
                sampleNumber = boost::lexical_cast<int>(modifiers);
            }
        }
    }
}