//
// Created by MasterLogick on 1/25/20.
//

#include "Input.h"

namespace osu {
    std::thread Input::inputThread = std::thread();
    KeyBindingStore Input::store = KeyBindingStore();

    void Input::initialise() {
        std::string keyBindings = ((std::string) (*Properties::KeyBindings));
        loadKeyBindingStoreFromString(&keyBindings);
    }

    void Input::loadKeyBindingStoreFromString(std::string *keyBindingStore) {
        std::string source = *keyBindingStore;
        std::vector<std::string> splitted;
        boost::algorithm::split(splitted, source, boost::is_any_of(";"), boost::token_compress_on);
        for (auto i = splitted.begin(); i != splitted.end(); ++i) {
            if (i->size() == 0)continue;
            std::vector<std::string> binding;
            boost::algorithm::split(binding, *i, boost::is_any_of("=+"), boost::token_compress_on);
            KeyBinding bind;
            bind.first = stoul(binding[0], nullptr, 10);
            KeyGroup group;
            group.size = binding.size() - 1;
            Key *keys = new Key[group.size];
            for (int j = 0; j < group.size; ++j) {
                keys[j] = stoul(binding[j + 1], nullptr, 10);
            }
            group.keys = keys;
            bind.second = group;
            store.add(&bind);
            delete[] keys;
        }
    }
}