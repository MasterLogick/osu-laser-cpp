#include "logic/Osu.h"


int main(int argc, const char **argv, const char **envp) {
#ifndef NDEBUG
    std::cout << "Compilation date and time: " << __DATE__ << " " << __TIME__ << std::endl << "Dev build" << std::endl;
#endif
    osu::Osu::initialise();
    osu::Osu::start();
}