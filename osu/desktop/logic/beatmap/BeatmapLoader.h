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

namespace osu {
    class BeatmapLoader {
    private:
//        Beatmap *beatmap;
        BeatmapMetadata *metadata;
        TimingPointSet *timingQueue;
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
        int formatVersion;
        int globalOffset;
        std::vector<std::pair<std::string, std::string>> variables;
        SectionTokens currientToken{General};

        void handleGeneral(std::string line);

        void handleEditor(std::string line);

        void handleMetadata(std::string line);

        void handleDifficulty(std::string line);

        void handleEvents(std::string line);

        void handleTimingPoints(std::string line);

        void handleColours(std::string line);

        void handleHitObjects(std::string line);

        void handleVariables(std::vector<std::pair<std::string, std::string>> *map, std::string line);

        void decodeVariables(std::string *line);

        int getOffsetTime(int time);

        double getOffsetTime();

        double getOffsetTime(double time);

        void addControlPoint(TimingPoint timingPoint);

    public:

        BeatmapLoader(int version) : formatVersion(version), globalOffset(version < 5 ? 24 : 0) {};

        void loadLegacyBeatmapFromFile(std::string path);

        void loadLegacyStoryBoardFromFile(std::string path);

        Beatmap *buildBeatmap();

    };
}

#endif //OSU_LASER_C_BEATMAPLOADER_H
