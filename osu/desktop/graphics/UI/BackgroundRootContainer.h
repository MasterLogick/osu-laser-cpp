//
// Created by user on 2/3/20.
//

#ifndef OSU_LASER_C_BACKGROUNDROOTCONTAINER_H
#define OSU_LASER_C_BACKGROUNDROOTCONTAINER_H

#include "TriangleBackground.h"
#include "Component.h"

#define TRIANGLES_AMOUNT 1000
namespace osu {
    class BackgroundRootContainer : Component {
    private:
        void draw(int x, int y) override {
            triangleBackground.draw(0, 0);
        }

        TriangleBackground triangleBackground;
    public:
        BackgroundRootContainer() : triangleBackground(1000) {

        }

        void draw() {
            draw(0, 0);
        }

        void initialise() {
            triangleBackground.initialise();
        }
    };
}

#endif //OSU_LASER_C_ROOTCONTAINER_H
