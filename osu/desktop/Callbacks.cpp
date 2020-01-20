//
// Created by user on 1/20/20.
//
#include <iostream>
#include "Osu.cpp"

namespace osu {
    void errorCallback(int error_code, const char *description) {
        std::cout << "Error code: " << error_code << "\n" << description << std::endl;
    }

    void closeCallback(GLFWwindow *window) {
        shouldClose = true;
    }
}