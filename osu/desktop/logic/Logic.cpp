//
// Created by MasterLogick on 9/18/20.
//

#include "Logic.h"
#include "../graphics/game_modes/standart/GameScreenDrawable.h"
#include "../graphics/GameDrawer.h"

namespace osu {
    Beatmap *Logic::CurrentBeatmap = nullptr;

    void Logic::initialise() {
        BeatmapLoader bl;
        bl.loadLegacyBeatmap(
                "/home/user/.local/share/wineprefixes/osuWine/drive_c/users/user/osu/Songs/54675 Groove Coverage - Holy Virgin (Nightcore Mix)/Groove Coverage - Holy Virgin (Nightcore Mix) (-Bakari-) [Gabe's Easy].osu");
//        bl.loadLegacyStoryboard(
//                "/home/user/.local/share/wineprefixes/osuWine/drive_c/users/user/osu/Songs/54675 Groove Coverage - Holy Virgin (Nightcore Mix)/Groove Coverage - Holy Virgin (Nightcore Mix) (-Bakari-).osb");
        CurrentBeatmap = bl.buildBeatmap();
        GameScreenDrawable gsd(CurrentBeatmap);
        GameDrawer::useGameScreenDrawable(&gsd);
    }

    void Logic::start() {

    }
}