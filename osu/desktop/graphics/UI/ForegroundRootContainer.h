//
// Created by MasterLogick on 2/11/20.
//

#ifndef OSU_LASER_C_FOREGROUNDROOTCONTAINER_H
#define OSU_LASER_C_FOREGROUNDROOTCONTAINER_H

#include <string>
#include "Component.h"

namespace osu {
    class ForegroundRootContainer : Component {
    private:
        void draw(int x, int y) override;

        std::wstring fps;
    public:
        ForegroundRootContainer();

        void draw();

        void initialise();
    };
}

#endif //OSU_LASER_C_FOREGROUNDROOTCONTAINER_H
