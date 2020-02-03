//
// Created by user on 2/1/20.
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
        static bool equals(KeyBinding a, KeyBinding b) {
            if (a.first == b.first && a.second.size == b.second.size) {
                for (int i = 0; i < a.second.size; ++i) {
                    if (a.second.keys[i] != b.second.keys[i])return false;
                }
                return true;
            }
            return false;
        }

        std::vector<std::vector<KeyBinding> *> bindings;
        double *keys;
    public:
        KeyBindingStore() {
            keys = new double[GLFW_KEY_LAST];
            for (int i = 0; i < GLFW_KEY_LAST; ++i) {
                bindings.push_back(new std::vector<KeyBinding>());
            }
        }

        void add(KeyBinding binding) {
            Key *pointer = new Key[binding.second.size];
            std::memcpy(pointer, binding.second.keys, binding.second.size * sizeof(Key));
            binding.second.keys = pointer;
            bindings[binding.second.keys[0]]->push_back(binding);
        }

        void update(KeyBinding replacement, KeyGroup replacing) {
            Key *pointer = new Key[replacing.size];
            std::memcpy(pointer, replacing.keys, replacing.size * sizeof(Key));
            replacing.keys = pointer;
            std::vector<KeyBinding> *keyBindings = bindings[replacement.second.keys[0]];
            for (auto i = keyBindings->begin(); i != keyBindings->end(); ++i) {
                KeyBinding keyBinding = *i;
                if (equals(keyBinding, replacement)) {
                    i->second = replacing;
                    return;
                }
            }
        }

        void remove(KeyBinding removing) {
            std::vector<KeyBinding> *keyBindings = bindings[removing.second.keys[0]];
            for (auto iter = keyBindings->begin(); iter != keyBindings->end(); iter++) {
                if (equals(removing, *iter)) {
                    delete[] iter->second.keys;
                    keyBindings->erase(iter);
                    return;
                }
            }
        }

        const char *getStateStr() {
            std::stringstream state;
            for (auto i = bindings.begin(); i != bindings.end(); i++) {
                std::vector<KeyBinding> *keyBindings = *i;
                if (keyBindings->size() != 0) {
                    for (auto i1 = keyBindings->begin(); i1 != keyBindings->end(); i1++) {
                        state << i1->first << "=";
                        for (int j = 0; j < i1->second.size - 1; ++j) {
                            state << i1->second.keys[j] << "+";
                        }
                        state << i1->second.keys[i1->second.size - 1] << ";";
                    }
                }
            }
            std::string *s = new std::string(state.str());
            return s->c_str();
        }
    };
}
#endif //OSU_LASER_C_KEYBINDINGSTORE_H
