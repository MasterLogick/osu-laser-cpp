//
// Created by Masterlogick on 4/3/20.
//

#ifndef OSU_LASER_C_BEATMAPLOADER_H
#define OSU_LASER_C_BEATMAPLOADER_H

#include <string>
#include "Beatmap.h"
#include "BeatmapMetadata.h"
#include "TimingPoint.h"
#include "TimingPointSet.h"
#include "ColorSchema.h"
#include "../modes/HitObjectParser.h"

namespace osu {
    class BeatmapLoader {
    private:
        enum SectionTokens {
            General,
            Editor,
            Metadata,
            Difficulty,
            Events,
            TimingPoints,
            Colours,
            HitObjects,
            Variables,
            None
        };
        BeatmapMetadata *metadata;
        TimingPointSet *timingPointSet;
        ColorSchema *colorSchema;
        HitObjectParser *hitObjectParser;
        std::list<std::pair<std::string, std::string>> variables;
        std::vector<HitObject *> hitObjects;
        int formatVersion;
        int globalOffset;
        SectionTokens currientToken{None};

        void handleSection(std::string &line);

        void handleGeneral(std::string &line);

        void handleEditor(std::string &line);

        void handleMetadata(std::string &line);

        void handleDifficulty(std::string &line);

        void handleEvents(std::string &line);

        void handleTimingPoints(std::string &line);

        void handleColours(std::string &line);

        void handleHitObjects(std::string &line);

        void handleVariables(std::string &line);

        void decodeVariables(std::string *line);

        int getOffsetTime(int time);

        double getOffsetTime();

        double getOffsetTime(double time);

        void addControlPoint(TimingPoint timingPoint);

    public:

        explicit BeatmapLoader(int version);

        void loadLegacyBeatmap(std::ifstream &stream);

        void loadLegacyStoryBoardFromFile(std::istream &stream);

        Beatmap *buildBeatmap();

        ///Removes all pointers and flushes loading variables. Prepares loader for next beatmap
        void prepare();

        void setVersion(int version);

        ///Destroys all available allocated data. Call it only on error. Built beatmap MUST destroy all related data
        void destroy();

    };
}

#endif //OSU_LASER_C_BEATMAPLOADER_H
