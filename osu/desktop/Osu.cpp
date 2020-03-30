#include "Osu.h"
#include "graphics/Graphics.h"
#include "interaction/Properties.h"
#include "logic/Logic.h"
#include "interaction/Interaction.h"
#include <boost/dll/runtime_symbol_info.hpp>
#include <locale>


int main(int argc, const char **argv, const char **envp) {
#ifndef NDEBUG
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }
    std::cout << boost::dll::program_location() << std::endl;
#endif
    osu::Osu::initialise();
    osu::Osu::start();
}

namespace osu {
    bool shouldClose = false;

    void Osu::initialise() {
        std::locale::global(std::locale("en_US.UTF-8"));
        Properties::initialise();
        Graphics::initialise();
        Logic::initialise();
        Interaction::initialise();
    }

    void Osu::start() {
        Logic::start();
        Interaction::start();
        Graphics::start();
#ifndef NDEBUG
        std::cout << "Join to drawingThread" << std::endl;
#endif
        Graphics::drawingThread->join();
    }
}