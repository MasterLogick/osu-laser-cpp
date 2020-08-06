//
// Created by MasterLogick on 7/1/20.
//

#include "CompoundCommand.h"

namespace osu {

    bool CompoundCommand::isCompound() {
        return true;
    }

    CompoundCommand::CompoundCommand() {
        type = CTEventContainer;
    }

    void CompoundCommand::addCommand(Command *command) {
        c.add(command);
    }

    void CompoundCommand::pack() {
        c.pack();
    }

    void CompoundCommand::process(int time) {
        Command *command;
        Command *old = nullptr;
        while ((command = c.get(time)) != old) {
            old = command;
            command->process(time);
        }
    }
}