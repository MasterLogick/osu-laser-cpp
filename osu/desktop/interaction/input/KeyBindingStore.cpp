//
// Created by Masterlogick on 2/5/20.
//
#include "KeyBindingStore.h"

namespace osu {
    bool KeyBindingStore::equals(KeyBinding *a, KeyBinding *b) {
        if (a->first == b->first && a->second.size == b->second.size) {
            for (int i = 0; i < a->second.size; ++i) {
                if (a->second.keys[i] != b->second.keys[i])return false;
            }
            return true;
        }
        return false;
    }

    KeyBindingStore::KeyBindingStore() {
        keys = new double[GLFW_KEY_LAST];
        for (int i = 0; i < GLFW_KEY_LAST; ++i) {
            bindings.push_back(new std::vector<KeyBinding>());
        }
    }

    void KeyBindingStore::add(KeyBinding *binding) {
        Key *pointer = new Key[binding->second.size];
        std::memcpy(pointer, binding->second.keys, binding->second.size * sizeof(Key));
        binding->second.keys = pointer;
        bindings[binding->second.keys[0]]->push_back(*binding);
    }

    void KeyBindingStore::update(KeyBinding *replacement, KeyGroup *replacing) {
        Key *pointer = new Key[replacing->size];
        std::memcpy(pointer, replacing->keys, replacing->size * sizeof(Key));
        replacing->keys = pointer;
        std::vector<KeyBinding> *keyBindings = bindings[replacement->second.keys[0]];
        for (auto i = keyBindings->begin(); i != keyBindings->end(); ++i) {
            KeyBinding keyBinding = *i;
            if (equals(&keyBinding, replacement)) {
                i->second = *replacing;
                return;
            }
        }
    }

    void KeyBindingStore::remove(KeyBinding *removing) {
        std::vector<KeyBinding> *keyBindings = bindings[removing->second.keys[0]];
        for (auto iter = keyBindings->begin(); iter != keyBindings->end(); iter++) {
            if (equals(removing, &*iter)) {
                delete[] iter->second.keys;
                keyBindings->erase(iter);
                return;
            }
        }
    }

    const char *KeyBindingStore::getStateStr() {
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
}