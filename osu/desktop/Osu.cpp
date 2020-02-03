#include "Osu.h"
#include "graphics/Graphics.h"
#include "interaction/Properties.h"
#include "logic/Logic.h"
#include "interaction/Interaction.h"
#include <boost/dll/runtime_symbol_info.hpp>
bool osu::shouldClose = false;

int main(int argc, const char **argv, const char **envp) {
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }
    std::cout << boost::dll::program_location() << std::endl;
    osu::Osu::initialise();
    osu::Osu::start();
    return EXIT_SUCCESS;
}

namespace osu {
    void Osu::initialise() {
        srand(time(nullptr));
        Properties::initialise();
        Graphics::initialise();
        Logic::initialise();
        Interaction::initialise();
    }

    void Osu::start() {
        Logic::start();
        Interaction::start();
        Graphics::start();
        std::cout << "Join to drawingThread" << std::endl;
//        while (true)std::this_thread::yield();
        Graphics::drawingThread->join();
    }
}