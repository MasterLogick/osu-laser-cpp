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
        std::unique_ptr<MovieState> movState;
    public:
        explicit VideoPlayer(std::string filePath);
    };
}


#endif //OSU_LASER_CPP_VIDEOPLAYER_H
