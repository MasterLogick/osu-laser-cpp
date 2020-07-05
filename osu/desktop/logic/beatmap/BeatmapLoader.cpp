//
// Created by MasterLogick on 4/3/20.
//
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <utility>
#include <string>
#include "BeatmapLoader.h"
#include "../modes/osu/OsuHitObjectParser.h"
#include "../utill/StringUtills.h"
#include "../utill/str_switch.h"
#include "components/BeatmapMetadata.h"
#include "components/BeatmapEnums.h"
#include "components/TimingPoint.h"
#include "components/storyboard/Animation.h"
#include "components/storyboard/Break.h"
#include "components/storyboard/Background.h"
#include "components/storyboard/Sample.h"
#include "components/storyboard/Sprite.h"
#include "components/storyboard/Video.h"
#include "../utill/IOUtills.h"
#include "components/storyboard/commands/Colour.h"
#include "components/storyboard/commands/Fade.h"
#include "components/storyboard/commands/Loop.h"
#include "components/storyboard/commands/Move.h"
#include "components/storyboard/commands/MoveX.h"
#include "components/storyboard/commands/MoveY.h"
#include "components/storyboard/commands/VectorScale.h"
#include "components/storyboard/commands/Scale.h"
#include "components/storyboard/commands/Trigger.h"
#include "components/storyboard/commands/Rotate.h"
#include "components/storyboard/commands/Parameter.h"

namespace osu {


    void BeatmapLoader::loadLegacyBeatmap(std::ifstream &stream) {
        std::string line;
        while (!stream.eof()) {
            readLineMultiplatform(stream, line);
#ifndef NDEBUG
            if (line.empty()) {
                //todo catch exception in readLineMultiplatform
                return;
            }
#else
#error catch exception in readLineMultiplatform
#endif
            trim(line);
            std::size_t pos = line.find("//");
            if (line.size() <= 1 || pos == 0) {
                continue;
            }
            if (line[0] == '[' && line[line.size() - 1] == ']') {
                handleSection(line);
            } else {
                switch (currentToken) {
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
                    case Colours:
                        handleColours(line);
                        break;
                    case None:
                        //todo through exception
                        break;
                }
            }
        }
    }

    void BeatmapLoader::handleSection(std::string &line) {
        sswitch(line) {
            scase("[General]"):
                currentToken = General;
                break;
                scase("[Editor]"):
                currentToken = Editor;
                break;
                scase("[Metadata]"):
                currentToken = Metadata;
                break;
                scase("[Difficulty]"):
                currentToken = Difficulty;
                break;
                scase("[Events]"):
                currentToken = Events;
                break;
                scase("[TimingPoints]"):
                currentToken = TimingPoints;
                break;
                scase("[Colours]"):
                currentToken = Colours;
                break;
                scase("[HitObjects]"):
                currentToken = HitObjects;
                break;
                scase("[Variables]"):
                currentToken = Variables;
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
        beatmap->metadata = metadata;
        beatmap->colorSchema = colorSchema;
        beatmap->timingPointSet = timingPointSet;
        return beatmap;
    }

    void BeatmapLoader::prepare() {
        currentToken = None;
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

    BeatmapLoader::~BeatmapLoader() {
        //todo implement
    }

    void BeatmapLoader::loadLegacyStoryboardFromFile(std::istream &stream) {
        currentToken = None;
        std::string line;
        while (!stream.eof()) {
            readLineMultiplatform(stream, line);
            if (line.empty() || startsWith(line, "//")) {
                continue;
            }
            if (line.front() == '[' && line.back() == ']') {
                handleSection(line);
            } else {
                switch (currentToken) {
                    case Events:
                        handleEvents(line);
                        break;
                    case Variables:
                        handleVariables(line);
                        break;
                    case None:
                        //todo through exception
                        break;
                }
            }
        }
    }

    void BeatmapLoader::handleVariables(std::string &line) {
        variables.push_back(splitKeyValPair(line, '='));
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

    void BeatmapLoader::handleEvents(std::string &line) {
        depth = 0;
        const char *dataptr = line.c_str();
        for (int i = 0, s = line.size(); s > i && (dataptr[i] == ' ' || dataptr[i] == '_'); ++i) {
            depth++;
        }
        line = line.substr(depth);
        while (depth < commandStack.size()) {
            commandStack.top()->commit();
            commandStack.pop();
        }
        decodeVariables(&line);
        if (depth == 0) {
            if (currentEventContainer != nullptr) {
                storyboard->addEventCommandContainer(currentEventContainer);
            }
            Event *event = parseEvent(line);
            storyboard->addEvent(event);
            currentEvent = event;
            currentEventContainer = new CompoundCommand();
            currentEventContainer->setParent(currentEvent);
            commandStack.push(currentEventContainer);
        } else {
            Command *command = parseCommand(line);
            command->setParent(currentEvent);
            commandStack.top()->addCommand(command);
            if (command->isCompound()) {
                commandStack.push(static_cast<CompoundCommand *>(command));
            }
        }
    }

    Event *BeatmapLoader::parseEvent(std::string &line) {
        std::vector<std::string> data = split(line, ",");
        EventType type = parseEventType(data[0]);
        switch (type) {
            case EventType::ETAnimation:
                return new Animation(data);
            case EventType::ETBreak:
                return new Break(data);
            case EventType::ETBackground:
                return new Background(data);
            case EventType::ETSample:
                return new Sample(data);
            case EventType::ETSprite:
                return new Sprite(data);
            case EventType::ETVideo:
                return new Video(data);
        }
    }

    Command *BeatmapLoader::parseCommand(std::string &line) {
        std::vector<std::string> data = split(line, ",");
        CommandType type = parseCommandType(data[0]);
        switch (type) {
            case CTColour :
                return new Colour(data);
            case CTFade :
                return new Fade(data);
            case CTLoop :
                return new Loop(data);
            case CTMove :
                return new Move(data);
            case CTMoveX :
                return new MoveX(data);
            case CTMoveY :
                return new MoveY(data);
            case CTRotate :
                return new Rotate(data);
            case CTScale :
                return new Scale(data);
            case CTTrigger :
                return new Trigger(data);
            case CTVectorScale :
                return new VectorScale(data);
            case CTParameter:
                return new Parameter(data);
        }
    }
}