//
// Created by MasterLogick on 2/9/20.
//
#include "logic/utill/StringUtills.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <boost/dll/runtime_symbol_info.hpp>
#include <logic/beatmap/BeatmapLoader.h>

int main(int argc, const char **argv, const char **envp) {
#ifndef NDEBUG
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }
    std::cout << boost::dll::program_location() << std::endl;
#endif
    osu::BeatmapLoader bl(14);
    std::ifstream in;
    in.open("/home/user/Downloads/1171789 SPYAIR - Imagination (TV Size)/SPYAIR - Imagination (TV Size) (browiec) [Normal].osu");
    bl.loadLegacyBeatmap(in);
    osu::Beatmap *b = bl.buildBeatmap();
    std::cout << b->metadata->Metadata.Title << std::endl;
//    std::cout << sizeof(osuSample) << std::endl << sizeof(int);
    return 0;
}
