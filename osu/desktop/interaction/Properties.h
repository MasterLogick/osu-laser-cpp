//
// Created by user on 1/18/20.
//

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

