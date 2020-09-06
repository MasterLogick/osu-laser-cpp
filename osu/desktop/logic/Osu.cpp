//
// Created by MasterLogick on 6/18/20.
//
#include <boost/filesystem/path.hpp>
#include "Osu.h"

namespace osu {
    bool shouldClose = false;

    void Osu::initialise() {
        std::locale::global(std::locale("en_US.UTF-8"));
        Properties::initialise();
        AudioSystem::initialise();
        Graphics::initialise();
        Logic::initialise();
    }

    void Osu::start() {
        Logic::start();
        Graphics::start();
#ifndef NDEBUG
        std::cout << "Join to drawingThread" << std::endl;
#endif
        Graphics::drawingThread->join();
    }
}