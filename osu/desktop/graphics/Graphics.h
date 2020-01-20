//
// Created by user on 1/19/20.
//


#include "GLFWWindow.h"
#include "GameDrawer.h"
#include "UIManager.h"

namespace osu {
    class Graphics {
    private:
        static void initGraphics();

        static void init();

    public:
        static GLFWWindow *mainScreen;

        static void initialise();

        static void start();
    };
}
