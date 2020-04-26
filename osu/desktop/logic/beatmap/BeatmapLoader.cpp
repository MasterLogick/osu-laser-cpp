//
// Created by Masterlogick on 4/3/20.
//
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "BeatmapLoader.h"
#include "BeatmapMetadata.h"
#include "../utill/StringUtills.h"
#include "../utill/str_switch.h"
#include "BeatmapEnums.h"
#include "TimingPoint.h"

#define KIAI_MODE 0x1
#define OMIT_FIRST_BAR_SIGNATURE 0x8
namespace osu {


    void BeatmapLoader::loadLegacyBeatmapFromFile(std::string path) {
        BeatmapMetadata *bm = new BeatmapMetadata();
        std::ifstream stream;
        stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            stream.open(path);
        }
        catch (std::ifstream::failure e) {
            std::cout << "Beatmap metadata loading error:" << e.what() << std::endl;
            return;
        }
        std::string line;
        while (!stream.eof()) {
            getline(stream, line);
            ltrim(line);
            std::size_t pos = line.find("//");
            if (line.size() <= 1 || pos == 0) {
                continue;
            }
            if (line[0] == '[' && line[line.size() - 1] == ']') {
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
                    case Colours:
                        handleColours(line);
                        break;
                    case HitObjects:
                        handleHitObjects(line);
                        break;
                    case Variables:
                        handleVariables(&variables, line);
                        break;
                }
            }
        }
    }

    void BeatmapLoader::handleGeneral(std::string line) {
        std::pair<std::string, std::string> data = splitKeyValPair(line);
        switch (line.size()) {
            case (1):
                metadata = 0;
                break;
        }
        sswitch(data.first) {
            scase("AudioFilename"):
                metadata->General.AudioFilename = const_cast<char *>(data.second.c_str());
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
                metadata->General.SkinPreference = const_cast<char *>(data.second.c_str());
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

    void BeatmapLoader::handleEditor(std::string line) {
        std::pair<std::string, std::string> data = splitKeyValPair(line);
        sswitch(data.first) {
            scase("Bookmarks"):
                metadata->Editor.Bookmarks = const_cast<char *>(data.second.c_str());
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

    void BeatmapLoader::handleMetadata(std::string line) {
        std::pair<std::string, std::string> data = splitKeyValPair(line);
        sswitch(data.first) {
            scase("Artist"):
                metadata->Metadata.Artist = const_cast<char *>(data.second.c_str());
                break;
                scase("ArtistUnicode"):
                metadata->Metadata.ArtistUnicode = const_cast<char *>(data.second.c_str());
                break;
                scase("BeatmapID"):
                metadata->Metadata.BeatmapID = boost::lexical_cast<long long int>(data.second);
                break;
                scase("BeatmapSetID"):
                metadata->Metadata.BeatmapSetID = boost::lexical_cast<long long int>(data.second);
                break;
                scase("Creator"):
                metadata->Metadata.Creator = const_cast<char *>(data.second.c_str());
                break;
                scase("Source"):
                metadata->Metadata.Source = const_cast<char *>(data.second.c_str());
                break;
                scase("Tags"):
                metadata->Metadata.Tags = const_cast<char *>(data.second.c_str());
                break;
                scase("Title"):
                metadata->Metadata.Title = const_cast<char *>(data.second.c_str());
                break;
                scase("TitleUnicode"):
                metadata->Metadata.TitleUnicode = const_cast<char *>(data.second.c_str());
                break;
                scase("Version"):
                metadata->Metadata.Version = const_cast<char *>(data.second.c_str());
                break;
        }
    }

    void BeatmapLoader::handleDifficulty(std::string line) {
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

    void BeatmapLoader::handleEvents(std::string line) {
        decodeVariables(&line);
    }

    void BeatmapLoader::handleTimingPoints(std::string line) {
        std::vector<std::string> vals = split(line, ",");
        size_t valsCount = vals.size();
        TimingPoint point{metadata->General.SampleSet};
        point.time = getOffsetTime(boost::lexical_cast<int>(trim_copy(vals[0])));
        /*double beatLength = boost::lexical_cast<double>(trim_copy(vals[1]));
        double speedMultiplier = beatLength < 0 ? -100.0 / beatLength : 1;
        TimeSignatures timeSignature{Quadruple};
        SampleSets sampleSet{metadata->General.SampleSet};
        int sampleIndex{0};
        int sampleVolume{100};
        bool uninherited{true};
        bool kiaiMode = false;
        bool omitFirstBarSignature = false;*/
        if (valsCount >= 3) {
            point.timeSignature = vals[2][0] == '0' ? Quadruple : (TimeSignatures) boost::lexical_cast<int>(trim_copy(vals[2]));
            if (valsCount >= 4) {
                point.sampleSet = vals[3][0] == '0' ? point.sampleSet : (SampleSets) boost::lexical_cast<int>(trim_copy(vals[3]));
                if (valsCount >= 5) {
                    point.sampleIndex = boost::lexical_cast<int>(trim_copy(vals[4]));
                    if (valsCount >= 6) {
                        point.sampleVolume = boost::lexical_cast<int>(trim_copy(vals[5]));
                        if (valsCount >= 7) {
                            point.uninherited = boost::lexical_cast<bool>(trim_copy(vals[6]));
                            if (valsCount >= 8) {
                                int bitField = boost::lexical_cast<int>(trim_copy(vals[7]));
                                point.kiaiMode = bitField & KIAI_MODE;
                                point.omitFirstBarSignature = bitField & OMIT_FIRST_BAR_SIGNATURE;
                            }
                        }
                    }
                }
            }
        }
        addControlPoint(point);
    }

    void BeatmapLoader::handleColours(std::string line) {

    }

    void BeatmapLoader::handleHitObjects(std::string line) {

    }

    void BeatmapLoader::handleVariables(std::vector<std::pair<std::string, std::string>> *map, std::string line) {
        map->push_back(splitKeyValPair(line, '='));
    }

    void BeatmapLoader::decodeVariables(std::string *line) {
        while (line->find('$') != std::string::npos) {
            char copys[line->length()];
            line->copy(copys, line->length());
            std::for_each(variables.begin(), variables.end(), [line](std::pair<std::string, std::string> &i) {
                boost::replace_all(*line, i.first.c_str(), i.second.c_str());
            });
            if (!line->compare(copys))return;
        }
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
        timingQueue->timingPointQueue.push(timingPoint);
    }
}