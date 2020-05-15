//
// Created by Masterlogick on 5/5/20.
//

#include <boost/lexical_cast.hpp>
#include "OsuHitObjectParser.h"
#include "../../utill/StringUtills.h"
#include "../../utill/Point.h"
#include "OsuHitObject.h"

namespace osu {

    void *OsuHitObjectParser::parseHitObject(std::string line) {
        std::vector<std::string> data = split(line, ",");
        OsuHitObject *obj{nullptr};
        int type = boost::lexical_cast<int>(data[3]);
        if (type & OsuHitObjectType::Circle) {
            obj = new OsuCircle();
        } else if (type & OsuHitObjectType::Slider) {
            obj = new OsuSlider();
        } else if (type & OsuHitObjectType::Spinner) {
            obj = new OsuSpinner();
        } else {
            //throw unknown_type error and return
        }
        obj->pos.x = boost::lexical_cast<int>(data[0]);
        obj->pos.y = boost::lexical_cast<int>(data[1]);
        obj->time = boost::lexical_cast<int>(data[2]);
    }
}