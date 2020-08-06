//
// Created by MasterLogick on 7/1/20.
//

#ifndef OSU_LASER_CPP_COMPOUNDCOMMAND_H
#define OSU_LASER_CPP_COMPOUNDCOMMAND_H

#include "Command.h"
#include "../CommandContainer.h"

namespace osu {
    class CompoundCommand : public Command {
    protected:
        CommandContainer c;
    public:
        virtual void pack();

        void addCommand(Command *command);

        CompoundCommand();

        bool isCompound() override;

        void process(int time);
    };
}

#endif //OSU_LASER_CPP_COMPOUNDCOMMAND_H
