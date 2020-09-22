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
        endTime = c.getEndTime();
        parent->startTime = startTime = c.getStartTime();
    }

    void CompoundCommand::process(int time) {
        c.process(time);
    }

    bool CompoundCommand::empty() {
        return c.empty();
    }
}