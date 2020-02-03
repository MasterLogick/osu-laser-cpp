//
// Created by user on 1/25/20.
//

#include "Input.h"

namespace osu {
    std::thread Input::inputThread = std::thread();
    KeyBindingStore Input::store = KeyBindingStore();
}