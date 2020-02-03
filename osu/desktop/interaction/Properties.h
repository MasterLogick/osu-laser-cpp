//
// Created by user on 1/18/20.
//
#ifndef OSU_LASER_C_PROPERTIES_H
#define OSU_LASER_C_PROPERTIES_H

#include <libconfig.h++>

using namespace libconfig;

namespace osu {
    class Properties {
    private:
        static Config cfg;
    public:
        static Setting *VideoMode;
        static Setting *Title;
        static Setting *MSAALevel;
        static Setting *KeyBindings;

        static void initialise();
    };

}
#endif

