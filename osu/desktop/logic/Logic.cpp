//
// Created by MasterLogick on 9/18/20.
//

#include "Logic.h"
#include "../graphics/game_modes/standart/GameScreenDrawable.h"
#include "../graphics/GameDrawer.h"

namespace osu {

    void Logic::initialise() {
        BeatmapLoader *bl = new BeatmapLoader();
        bl->loadLegacyBeatmap(
                "/home/user/.local/share/wineprefixes/osuWine/drive_c/users/user/osu/Songs/242360 senya - Akatsuki no Suiheisen ni/senya - Akatsuki no Suiheisen ni (Satellite) [Catharsis].osu");
//        bl.loadLegacyStoryboard(
//                "/home/user/.local/share/wineprefixes/osuWine/drive_c/users/user/osu/Songs/54675 Groove Coverage - Holy Virgin (Nightcore Mix)/Groove Coverage - Holy Virgin (Nightcore Mix) (-Bakari-).osb");
        GameScreenDrawable *gsd = new GameScreenDrawable(bl->buildBeatmap());
        GameDrawer::useGameScreenDrawable(gsd);
    }

    void Logic::start() {

    }
}