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
            Variables
        };
//        Beatmap *beatmap;
        BeatmapMetadata *metadata{nullptr};
        TimingPointSet *timingQueue{nullptr};
        ColorSchema *colorSchema{nullptr};
        HitObjectParser *hitObjectParser{nullptr};
        std::list<std::pair<std::string, std::string>> variables;
        std::vector<void *> hitObjects;
        int formatVersion;
        int globalOffset;
        SectionTokens currientToken{General};

        void handleSection(std::string line);

        void handleGeneral(std::string line);

        void handleEditor(std::string line);

        void handleMetadata(std::string line);

        void handleDifficulty(std::string line);

        void handleEvents(std::string line);

        void handleTimingPoints(std::string line);

        void handleColours(std::string line);

        void handleHitObjects(std::string line);

        void handleVariables(std::string line);

        void decodeVariables(std::string *line);

        int getOffsetTime(int time);

        double getOffsetTime();

        double getOffsetTime(double time);

        void addControlPoint(TimingPoint timingPoint);

    public:

        explicit BeatmapLoader(int version);

        void loadLegacyBeatmap(std::istream stream);

        void loadLegacyStoryBoardFromFile(std::istream stream);

        Beatmap *buildBeatmap();

    };
}

#endif //OSU_LASER_C_BEATMAPLOADER_H
