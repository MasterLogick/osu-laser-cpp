//
// Created by user on 1/18/20.
//


#include <iostream>
#include <GLFW/glfw3.h>

#define TYPE_WINDOW 'W'
#define TYPE_BORDERLESS 'B'
#define TYPE_FULLSCREEN 'F'
namespace osu {
    class GLFWWindow {
    public:
        static void printPrimaryMonitorVideoModes();

        GLFWWindow();

        ~GLFWWindow();

        void setCurrent();

        void setVSync(int val);

        void showWindow();

        void hideWindow();

        void swapBuffers();

        bool isWindowShouldClose();

        void shouldClose();

        void freeCallbacks();

        void setKeyCallback(GLFWkeyfun callback);

        void setMouseCallback(GLFWcursorposfun callback);

        void setWindowShouldCloseCallback(GLFWwindowclosefun callback);

        void grabMouse();

        void releaseMouse();

        void setMousePos(double xpos, double ypos);

        int getWidth();

        int getHeight();

        void viewport();

        int getX();

        int getY();


    private:
        GLFWwindow *window;
        int width;
        int height;
    };
}
