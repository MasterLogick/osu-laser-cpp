//
// Created by MasterLogick on 2/1/20.
//

#ifndef OSU_LASER_C_KEYBINDINGSTORE_H
#define OSU_LASER_C_KEYBINDINGSTORE_H

#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>

namespace osu {
    typedef GLuint Key;
    struct KeyGroup {
        Key *keys;
        size_t size;
    };
    typedef unsigned int Action;
    typedef std::pair<Action, KeyGroup> KeyBinding;

    class KeyBindingStore {
    private:
        static bool equals(KeyBinding *a, KeyBinding *b);

        std::vector<std::vector<KeyBinding> *> bindings;
        double *keys;
    public:
        KeyBindingStore();

        void add(KeyBinding *binding);

        void update(KeyBinding *replacement, KeyGroup *replacing);

        void remove(KeyBinding *removing);

        const char *getStateStr();
    };
}
#endif //OSU_LASER_C_KEYBINDINGSTORE_H
