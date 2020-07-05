//
// Created by MasterLogick on 7/1/20.
//

#include "Command.h"

namespace osu {

    void Command::setParent(Event *parent) {
        this->parent = parent;
    }
}