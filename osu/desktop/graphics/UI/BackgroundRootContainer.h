//
// Created by MasterLogick on 2/3/20.
//

#ifndef OSU_LASER_C_BACKGROUNDROOTCONTAINER_H
#define OSU_LASER_C_BACKGROUNDROOTCONTAINER_H

#include "TriangleBackground.h"
#include "Component.h"

#define TRIANGLES_AMOUNT 1000
namespace osu {

    class BackgroundRootContainer : Component {
    private:
        void draw(int x, int y) override;

        TriangleBackground triangleBackground;
    public:

        BackgroundRootContainer() : triangleBackground(TRIANGLES_AMOUNT) {

        }

        void draw();

        void initialise();
    };
}

#endif //OSU_LASER_C_ROOTCONTAINER_H
