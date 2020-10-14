//
// Created by MasterLogick on 6/18/20.
//

#ifndef OSU_LASER_CPP_STORYBOARDBUILDER_H
#define OSU_LASER_CPP_STORYBOARDBUILDER_H

#include "Sprite.h"
#include "Animation.h"
#include "Sample.h"
#include "Event.h"
#include "commands/CompoundCommand.h"
#include "Storyboard.h"
#include "PreCachedImage.h"
#include "Timeline.h"
#include <list>
#include <vector>
#include <boost/filesystem/path.hpp>

namespace osu {
    using namespace boost::filesystem;

    class StoryboardBuilder {
    private:
        Timeline timeline;
//        std::vector<CompoundCommand *> commandContainers;
        std::vector<Event *> events;
        std::vector<PreCachedImage> texturePaths;
        path dir;

    public:
        StoryboardBuilder();

        StoryboardBuilder(path dir);

        void addEvent(Event *event);

        void addEventContainer(CompoundCommand *container);

        Storyboard *build();

        void setRootPath(path dir);

        path getRootPath();
    };
}

#endif //OSU_LASER_CPP_STORYBOARDBUILDER_H
