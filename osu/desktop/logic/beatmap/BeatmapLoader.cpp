//
// Created by Masterlogick on 4/3/20.
//
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <utility>
#include "BeatmapLoader.h"
#include "BeatmapMetadata.h"
#include "../utill/StringUtills.h"
#include "../utill/str_switch.h"
#include "BeatmapEnums.h"
#include "TimingPoint.h"
#include "../modes/osu/OsuHitObjectParser.h"

namespace osu {


    void BeatmapLoader::loadLegacyBeatmap(std::ifstream &stream) {
        std::string line;
        while (!stream.eof()) {
            getline(stream, line);
            trim(line);
            std::size_t pos = line.find("//");
            if (line.size() <= 1 || pos == 0) {
                continue;
            }
            if (line[0] == '[' && line[line.size() - 1] == ']') {
                handleSection(line);
            } else {
                switch (currientToken) {
                    case General:
                        handleGeneral(line);
                        break;
                    case Editor:
                        handleEditor(line);
                        break;
                    case Metadata:
                        handleMetadata(line);
                        break;
                    case Difficulty:
                        handleDifficulty(line);
                        break;
                    case Events:
                        handleEvents(line);
                        break;
                    case TimingPoints:
                        handleTimingPoints(line);
                        break;
                    case HitObjects:
                        handleHitObjects(line);
                        break;
                    case Variables:
                        handleVariables(line);
                        break;
                }
            }
        }
    }

    void BeatmapLoader::handleSection(std::string &line) {
        sswitch(line) {
            scase("[General]"):
                currientToken = General;
                break;
                scase("[Editor]"):
                currientToken = Editor;
                break;
                scase("[Metadata]"):
                currientToken = Metadata;
                break;
                scase("[Difficulty]"):
                currientToken = Difficulty;
                break;
                scase("[Events]"):
                currientToken = Events;
                break;
                scase("[TimingPoints]"):
                currientToken = TimingPoints;
                break;
                scase("[Colours]"):
                currientToken = Colours;
                break;
                scase("[HitObjects]"):
                currientToken = HitObjects;
                break;
                scase("[Variables]"):
                currientToken = Variables;
                break;
        }
    }

    void BeatmapLoader::handleGeneral(std::string &line) {
        std::pair<std::string, std::string> data = splitKeyValPair(line);
        sswitch(data.first) {
            scase("AudioFilename"):
                metadata->General.AudioFilename = std::move(data.second);
                break;
                scase("AudioLeadIn"):
                metadata->General.AudioLeadIn = boost::lexical_cast<long long int>(data.second);
                break;
                scase("Countdown"):
                metadata->General.Countdown = boost::lexical_cast<bool>(data.second);
                break;
                scase("CountdownOffset"):
                metadata->General.CountdownOffset = boost::lexical_cast<long long int>(data.second);
                break;
                scase("EpilepsyWarning"):
                metadata->General.EpilepsyWarning = boost::lexical_cast<bool>(data.second);
                break;
                scase("LetterboxInBreaks"):
                metadata->General.LetterboxInBreaks = boost::lexical_cast<bool>(data.second);
                break;
                scase("Mode"):
                metadata->General.Mode = static_cast<GameMode>(boost::lexical_cast<int>(data.second));
                switch (metadata->General.Mode) {
                    case Standard:
                        hitObjectParser = new OsuHitObjectParser(formatVersion);
                        //todo return standard parser
                        break;
                    case Taiko:
                        //todo return taiko parser
                        break;
                    case Mania:
                        //todo return mania parser
                        break;
                    case Catch:
                        //todo return catch parser
                        break;
                }
                break;
                scase("OverlayPosition"):
                sswitch(data.second) {
                    scase("NoChange"):
                        metadata->General.OverlayPosition = NoChange;
                        break;
                        scase("Below"):
                        metadata->General.OverlayPosition = Below;
                        break;
                        scase("Above"):
                        metadata->General.OverlayPosition = Above;
                        break;
                }
                break;
                scase("PreviewTime"):
                metadata->General.PreviewTime = boost::lexical_cast<long long int>(data.second);
                break;
                scase("SampleSet"):
                sswitch(data.second) {
                    scase("Normal"):
                        metadata->General.SampleSet = Normal;
                        break;
                        scase("Soft"):
                        metadata->General.SampleSet = Soft;
                        break;
                        scase("Drum"):
                        metadata->General.SampleSet = Drum;
                        break;
                }
                break;
                scase("SamplesMatchPlaybackRate"):
                metadata->General.SamplesMatchPlaybackRate = boost::lexical_cast<bool>(data.second);
                break;
                scase("SkinPreference"):
                metadata->General.SkinPreference = std::move(data.second);
                break;
                scase("SpecialStyle"):
                metadata->General.SpecialStyle = boost::lexical_cast<bool>(data.second);
                break;
                scase("StackLeniency"):
                metadata->General.StackLeniency = boost::lexical_cast<float>(data.second);
                break;
                scase("UseSkinSprites"):
                metadata->General.UseSkinSprites = boost::lexical_cast<bool>(data.second);
                break;
                scase("WidescreenStoryboard"):
                metadata->General.WidescreenStoryboard = boost::lexical_cast<bool>(data.second);
                break;
        }
    }

    void BeatmapLoader::handleEditor(std::string &line) {
        std::pair<std::string, std::string> data = splitKeyValPair(line);
        sswitch(data.first) {
            scase("Bookmarks"):
                metadata->Editor.Bookmarks = std::move(data.second);
                break;
                scase("BeatDivisor"):
                metadata->Editor.BeatDivisor = boost::lexical_cast<float>(data.second);
                break;
                scase("DistanceSpacing"):
                metadata->Editor.DistanceSpacing = boost::lexical_cast<float>(data.second);
                break;
                scase("GridSize"):
                metadata->Editor.GridSize = boost::lexical_cast<long long int>(data.second);
                break;
                scase("TimelineZoom"):
                metadata->Editor.TimelineZoom = boost::lexical_cast<float>(data.second);
                break;
        }
    }

    void BeatmapLoader::handleMetadata(std::string &line) {
        std::pair<std::string, std::string> data = splitKeyValPair(line);
        sswitch(data.first) {
            scase("Artist"):
                metadata->Metadata.Artist = std::move(data.second);
                break;
                scase("ArtistUnicode"):
                metadata->Metadata.ArtistUnicode = std::move(data.second);
                break;
                scase("BeatmapID"):
                metadata->Metadata.BeatmapID = boost::lexical_cast<long long int>(data.second);
                break;
                scase("BeatmapSetID"):
                metadata->Metadata.BeatmapSetID = boost::lexical_cast<long long int>(data.second);
                break;
                scase("Creator"):
                metadata->Metadata.Creator = std::move(data.second);
                break;
                scase("Source"):
                metadata->Metadata.Source = std::move(data.second);
                break;
                scase("Tags"):
                metadata->Metadata.Tags = std::move(data.second);
                break;
                scase("Title"):
                metadata->Metadata.Title = std::move(data.second);
                break;
                scase("TitleUnicode"):
                metadata->Metadata.TitleUnicode = std::move(data.second);
                break;
                scase("Version"):
                metadata->Metadata.Version = std::move(data.second);
                break;
        }
    }

    void BeatmapLoader::handleDifficulty(std::string &line) {
        std::pair<std::string, std::string> data = splitKeyValPair(line);
        sswitch(data.first) {
            scase("ApproachRate"):
                metadata->Difficulty.ApproachRate = boost::lexical_cast<float>(data.second);
                break;
                scase("CircleSize"):
                metadata->Difficulty.CircleSize = boost::lexical_cast<float>(data.second);
                break;
                scase("HPDrainRate"):
                metadata->Difficulty.HPDrainRate = boost::lexical_cast<float>(data.second);
                break;
                scase("OverallDifficulty"):
                metadata->Difficulty.OverallDifficulty = boost::lexical_cast<float>(data.second);
                break;
                scase("SliderMultiplier"):
                metadata->Difficulty.SliderMultiplier = boost::lexical_cast<float>(data.second);
                break;
                scase("SliderTickRate"):
                metadata->Difficulty.SliderTickRate = boost::lexical_cast<float>(data.second);
                break;
        }
    }

    void BeatmapLoader::handleTimingPoints(std::string &line) {
        std::vector<std::string> vals = split(line, ",");
        size_t valsCount = vals.size();
        TimingPoint point{metadata->General.SampleSet};
        point.time = getOffsetTime(boost::lexical_cast<int>(trim_copy(vals[0])));
        if (valsCount >= 3) {
            point.timeSignature =
                    vals[2][0] == '0' ? Quadruple : (TimeSignatures) boost::lexical_cast<int>(trim_copy(vals[2]));
            if (valsCount >= 4) {
                point.sampleSet =
                        vals[3][0] == '0' ? point.sampleSet : (SampleSets) boost::lexical_cast<int>(trim_copy(vals[3]));
                if (valsCount >= 5) {
                    point.sampleIndex = boost::lexical_cast<int>(trim_copy(vals[4]));
                    if (valsCount >= 6) {
                        point.sampleVolume = boost::lexical_cast<int>(trim_copy(vals[5]));
                        if (valsCount >= 7) {
                            point.uninherited = boost::lexical_cast<bool>(trim_copy(vals[6]));
                            if (valsCount >= 8) {
                                int bitField = boost::lexical_cast<int>(trim_copy(vals[7]));
                                point.kiaiMode = bitField & TimingPoint::KIAI_MODE;
                                point.omitFirstBarSignature = bitField & TimingPoint::OMIT_FIRST_BAR_SIGNATURE;
                            }
                        }
                    }
                }
            }
        }
        addControlPoint(point);
    }

    void BeatmapLoader::handleColours(std::string &line) {
        std::pair<std::string, std::string> data = splitKeyValPair(line);
        if (data.first.find("Combo") == 0) {
            std::vector<std::string> colorData = split(data.second, ",");
            Color c((uint8_t) boost::lexical_cast<int>(trim_copy(colorData[0])),
                    (uint8_t) boost::lexical_cast<int>(trim_copy(colorData[1])),
                    (uint8_t) boost::lexical_cast<int>(trim_copy(colorData[2])),
                    colorData.size() == 4 ? (uint8_t) boost::lexical_cast<int>(trim_copy(colorData[3])) : 255);
            colorSchema->ComboColors.push_back(c);
        } else if (data.first.find("SliderTrackOverride") == 0) {
            std::vector<std::string> colorData = split(data.second, ",");
            Color c((uint8_t) boost::lexical_cast<int>(trim_copy(colorData[0])),
                    (uint8_t) boost::lexical_cast<int>(trim_copy(colorData[1])),
                    (uint8_t) boost::lexical_cast<int>(trim_copy(colorData[2])),
                    colorData.size() == 4 ? (uint8_t) boost::lexical_cast<int>(trim_copy(colorData[3])) : 255);
            colorSchema->SliderTrackOverride = c;
        } else if (data.first.find("SliderBorder") == 0) {
            std::vector<std::string> colorData = split(data.second, ",");
            Color c((uint8_t) boost::lexical_cast<int>(trim_copy(colorData[0])),
                    (uint8_t) boost::lexical_cast<int>(trim_copy(colorData[1])),
                    (uint8_t) boost::lexical_cast<int>(trim_copy(colorData[2])),
                    colorData.size() == 4 ? (uint8_t) boost::lexical_cast<int>(trim_copy(colorData[3])) : 255);
            colorSchema->SliderBorder = c;
        }
    }

    void BeatmapLoader::handleHitObjects(std::string &line) {
        if (hitObjectParser == nullptr) {
            //todo catch undefined mode exception
        }
        HitObject *hitObject = hitObjectParser->parseHitObject(line);
        hitObjects.push_back(hitObject);
    }

    int BeatmapLoader::getOffsetTime(int time) {
        return time + globalOffset;
    }

    double BeatmapLoader::getOffsetTime() {
        return globalOffset;
    }

    double BeatmapLoader::getOffsetTime(double time) {
        return time + globalOffset;
    }

    void BeatmapLoader::addControlPoint(TimingPoint timingPoint) {
        timingPointSet->timingPointQueue.push_back(timingPoint);
    }

    BeatmapLoader::BeatmapLoader(int version) : formatVersion(version), globalOffset(version < 5 ? 24 : 0) {
        metadata = new BeatmapMetadata();
        timingPointSet = new TimingPointSet();
        colorSchema = new ColorSchema();
    }

    Beatmap *BeatmapLoader::buildBeatmap() {
        Beatmap *beatmap = new Beatmap();
        std::sort(hitObjects.begin(), hitObjects.end(),
                  [](HitObject *a, HitObject *b) -> bool {
                      return a->time < b->time;
                  });
        beatmap->hitObjects.insert(beatmap->hitObjects.cbegin(), hitObjects.begin(), hitObjects.end());
//        std::move(hitObjects.begin(), hitObjects.end(), beatmap->hitObjects.begin());
        beatmap->metadata = metadata;
        beatmap->colorSchema = colorSchema;
        beatmap->timingPointSet = timingPointSet;
        return beatmap;
    }

    void BeatmapLoader::prepare() {
        currientToken = None;
        metadata = new BeatmapMetadata();
        timingPointSet = new TimingPointSet();
        colorSchema = new ColorSchema();
        hitObjectParser = nullptr;
        variables.clear();
        hitObjects.clear();
    }

    void BeatmapLoader::setVersion(int version) {
        formatVersion = version;
        globalOffset = version < 5 ? 24 : 0;
    }

    void BeatmapLoader::destroy() {
        //todo
    }
}