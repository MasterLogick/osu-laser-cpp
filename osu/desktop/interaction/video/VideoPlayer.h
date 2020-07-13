//
// Created by MasterLogick on 7/10/20.
//

#ifndef OSU_LASER_CPP_VIDEOPLAYER_H
#define OSU_LASER_CPP_VIDEOPLAYER_H

#include <memory>
#include <string>
#include "MovieState.h"

namespace osu {
    class VideoPlayer {
        std::string filePath;
        std::unique_ptr<MovieState> movState;
        std::thread mainLoop;
    public:
        explicit VideoPlayer(std::string filePath);

        void initialise();

        void start();

        void stop();

        void pause();

        void setDirectOutMode(bool flag);

        void setWideStereo(bool flag);
    };
}


#endif //OSU_LASER_CPP_VIDEOPLAYER_H
