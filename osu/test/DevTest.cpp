//
// Created by MasterLogick on 20/06/2020.
//

#include <string>
#include <iostream>
#include <fstream>
#include <logic/beatmap/BeatmapLoader.h>
#include <TestResourcesPaths.h>

using namespace osu;

int main(int argc, const char **argv, const char **envp) {
    std::list<int> a;
    std::list<int>::iterator b{a.begin()};
    a.push_back(1);
    std::cout << *b << std::endl;
//    BeatmapLoader bl{14};
//    std::ifstream in{};
//    in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//    in.open(TEST_OSB_PATH);
//    bl.loadLegacyStoryboardFromFile(in);
}