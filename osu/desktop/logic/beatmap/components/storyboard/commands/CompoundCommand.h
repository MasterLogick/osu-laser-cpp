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
        virtual void commit();

        void addCommand(Command *command);

        explicit CompoundCommand();

        bool isCompound() override;
    };
}

#endif //OSU_LASER_CPP_COMPOUNDCOMMAND_H
