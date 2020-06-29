//
// Created by MasterLogick on 6/9/20.
//

#include "Sprite.h"
#include "../../../utill/StringUtills.h"
#include <boost/lexical_cast.hpp>

namespace osu {
    Sprite::Sprite(std::vector<std::string> &data) {
        //0      1       2        3            4   5
        //Sprite,(layer),(origin),”(filepath)”,(x),(y)
        if (data.size() != 6) {
            //todo throw unknown_type error
            //todo delete all allocated vars
        }
        layer = parseLayer(data[1]);
        origin = parseOrigin(data[2]);
        file = data[3].substr(data[3].front() == '"' ? 1 : 0,
                              data[3].size() - (data[3].front() == '"' ? 1 : 0) - (data[3].back() == '"' ? 1 : 0));
        pos.x = boost::lexical_cast<int>(data[4]);
        pos.y = boost::lexical_cast<int>(data[5]);
        eventType = EventType::ETSprite;
    }
}