//
// Created by MasterLogick on 2/3/20.
//

#ifndef OSU_LASER_C_COMPONENT_H
#define OSU_LASER_C_COMPONENT_H

namespace osu {
    class Component {
    public:
        virtual int getWidth() { return 0; }

        virtual int getHeight() { return 0; }

        virtual void draw(int x, int y) = 0;
    };
}

#endif //OSU_LASER_C_COMPONENT_H
