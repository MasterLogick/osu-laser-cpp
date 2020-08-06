//
// Created by MasterLogick on 5/5/20.
//

#include <boost/lexical_cast.hpp>
#include "OsuHitObjectParser.h"
#include "../../utill/StringUtills.h"
#include "../../utill/Point.h"
#include "OsuHitObject.h"

namespace osu {

    HitObject *OsuHitObjectParser::parseHitObject(std::string &line) {
        std::vector<std::string> data = split(line, ',');
        int type = boost::lexical_cast<int>(data[3]);
        if (type & HitObjectType::Circle) {
            //0 1 2    3    4        5
            //x,y,time,type,hitSound,hitSample
            if (data.size() < 5) {
                //todo throw unknown_type error
                //todo delete all allocated vars
                return nullptr;
            }
            OsuCircle *tmp = new OsuCircle();
            tmp->pos.x = boost::lexical_cast<int>(data[0]);
            tmp->pos.y = boost::lexical_cast<int>(data[1]);
            tmp->time = globalOffset + boost::lexical_cast<int>(data[2]);
            tmp->hitSoundBitField = boost::lexical_cast<int>(data[4]);
            if (data.size() < 6)
                tmp->hitSample = HitSample::DefaultHitsample;
            else
                tmp->hitSample = HitSample(data[5]);
            tmp->type = type;
            return tmp;
        } else if (type & HitObjectType::Slider) {
            //0 1 2    3    4        5         x:y         6      7      8          9        10
            //x,y,time,type,hitSound,curveType|curvePoints,slides,length,edgeSounds,edgeSets,hitSample
            if (data.size() < 8) {
                //todo throw unknown_type error
                //todo delete all allocated vars
                return nullptr;
            }
            OsuSlider *tmp = new OsuSlider();
            std::vector<std::string> path = split(data[5], '|');
            tmp->curveType = (CurveType) path[0][0];
            tmp->curvePoints = new Point[path.size()];
            tmp->curvePoints[0].x = boost::lexical_cast<int>(data[0]);
            tmp->curvePoints[0].y = boost::lexical_cast<int>(data[1]);
            for (int i = 1; i < path.size(); ++i) {
                std::pair<std::string, std::string> pnt = splitKeyValPair(path[i]);
                tmp->curvePoints[i].x = boost::lexical_cast<int>(pnt.first);
                tmp->curvePoints[i].y = boost::lexical_cast<int>(pnt.second);
            }
            tmp->curvePointsCount = path.size();
            tmp->slides = boost::lexical_cast<int>(data[6]);
            tmp->length = boost::lexical_cast<double>(data[7]);
            if (data.size() >= 10) {
                std::vector<std::string> edgeHitSounds = split(data[8], '|');
                std::vector<std::string> edgeHitSets = split(data[9], '|');
                if (edgeHitSounds.size() != edgeHitSets.size()) {
                    //todo throw incompatible_hitsound_sets error
                    //todo delete all allocated vars
                    return nullptr;
                }
                tmp->edgeCount = edgeHitSounds.size();
                tmp->edgeSounds = new uint8_t[edgeHitSounds.size()];
                for (int i = 0; i < edgeHitSounds.size(); ++i) {
                    tmp->edgeSounds[i] = boost::lexical_cast<int>(edgeHitSounds[i]);
                }
                tmp->edgeSets = new HitSample[edgeHitSets.size()];
                for (int i = 0; i < edgeHitSets.size(); ++i) {
                    tmp->edgeSets[i] = HitSample(edgeHitSets[i]);
                }
            } else {
                tmp->edgeCount = 0;
                tmp->edgeSounds = nullptr;
                tmp->edgeSets = nullptr;
            }
            tmp->time = globalOffset + boost::lexical_cast<int>(data[2]);
            tmp->hitSoundBitField = boost::lexical_cast<int>(data[4]);
            if (data.size() < 11)
                tmp->hitSample = HitSample::DefaultHitsample;
            else
                tmp->hitSample = HitSample(data[10]);
            tmp->type = type;
            return tmp;
        } else if (type & HitObjectType::Spinner) {
            //0 1 2    3    4        5       6
            //x,y,time,type,hitSound,endTime,hitSample
            if (data.size() < 6) {
                //todo throw incompatible_hitsound_sets error
                //todo delete all allocated vars
                return nullptr;
            }
            OsuSpinner *tmp = new OsuSpinner();
            tmp->time = globalOffset + boost::lexical_cast<int>(data[2]);
            tmp->endTime = boost::lexical_cast<int>(data[5]);
            tmp->hitSoundBitField = boost::lexical_cast<int>(data[4]);
            if (data.size() < 7)
                tmp->hitSample = HitSample::DefaultHitsample;
            else
                tmp->hitSample = HitSample(data[6]);
            tmp->type = type;
            return tmp;
        } else {
            //todo throw unknown_type error
            //todo delete all allocated vars
            return nullptr;
        }
    }
}