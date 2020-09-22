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
    ExecutionTimer _timer;
    std::cout << sizeof(Timeline);
    return 0;
    std::vector<int> a{0, 1, 2, 3, 4, 5};
    std::vector<int>::iterator b = a.begin();
    f(b);
    std::vector<int>::iterator c = std::vector<int>::iterator(b);
    b++;
//    std::cout << *c << " " << *b;
    std::for_each(a.begin(), ++a.begin(), [](int e) { std::cout << e; });
    return 0;
}
