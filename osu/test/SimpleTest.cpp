//
// Created by MasterLogick on 2/9/20.
//
#include <iostream>
#include <fstream>
#include <boost/dll/runtime_symbol_info.hpp>
#include <logic/beatmap/BeatmapLoader.h>
#include <TestResourcesPaths.h>
#include <logic/utill/BufferedReader.h>
#include "ExecutionTimer.h"

using namespace osu;

void f(std::vector<int>::iterator e) {
    e++;
}

int main(int argc, const char **argv, const char **envp) {
    for (int i = 10837; i <= 13920; ++i) {
        if ((i % 17 == 0) && (i % 7 != 0) && (i % 15 != 0) && (i % 18 != 0) && (i % 34 != 0)) {
            std::cout << i << std::endl;
        }
    }
}
