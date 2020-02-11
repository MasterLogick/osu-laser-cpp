//
// Created by MasterLogick on 1/25/20.
//

#ifndef OSU_LASER_C_INPUT_H
#define OSU_LASER_C_INPUT_H

#include <thread>
#include "KeyBindingStore.h"
#include "../Properties.h"
#include <boost/algorithm/string.hpp>

namespace osu {
    class Input {
    private:
        static std::thread inputThread;
        static KeyBindingStore store;

        static void loadKeyBindingStoreFromString(std::string *keyBindingStore) ;

    public:
        static void initialise();
    };
}

#endif //OSU_LASER_C_INPUT_H
