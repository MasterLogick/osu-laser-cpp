//
// Created by MasterLogick on 20/06/2020.
//

#include <string>
#include <iostream>
#include <fstream>
#include <logic/beatmap/BeatmapLoader.h>
#include <TestResourcesPaths.h>
#include <boost/lexical_cast.hpp>

using namespace osu;

int main(int argc, const char **argv, const char **envp) {
    BeatmapLoader bl{14};
    std::ifstream in{};
    in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    in.open("/home/user/.local/share/wineprefixes/osuWine/drive_c/users/user/osu/Songs/545737 REOL - YoiYoi Kokon/REOL - YoiYoi Kokon (DreaM117er).osb");
    bl.loadLegacyStoryboard(in);
}