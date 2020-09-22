//
// Created by MasterLogick on 20/06/2020.
//

#include <string>
#include <iostream>
#include <fstream>
#include <logic/beatmap/BeatmapLoader.h>
#include <TestResourcesPaths.h>
#include <boost/lexical_cast.hpp>
#include <chrono>
#include "ExecutionTimer.h"

using namespace osu;

int main(int argc, const char **argv, const char **envp) {
    ExecutionTimer _timer;
    BeatmapLoader bl;
    bl.loadLegacyStoryboard(
//            "/home/user/.local/share/wineprefixes/osuWine/drive_c/users/user/osu/Songs/171388 cillia - FIRST/cillia - FIRST (moph).osb");
            "/home/user/.local/share/wineprefixes/osuWine/drive_c/users/user/osu/Songs/470977 Mili - worldexecute(me);/Mili - world.execute(me); (Exile-).osb");
    bl.buildBeatmap();
}