//
// Created by MasterLogick on 1/23/20.
//
#ifndef OSU_LASER_C_OSU_H
#define OSU_LASER_C_OSU_H
namespace osu {
    extern bool shouldClose;

    class Osu {
    public:
        static void initialise();

        static void start();
    };
}

int main(int argc, const char **argv, const char **envp);

#endif