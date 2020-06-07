//
// Created by MasterLogick on 4/21/20.
//

#ifndef OSU_LASER_C_BEATMAPMETADATA_H
#define OSU_LASER_C_BEATMAPMETADATA_H

#include <cstdlib>
#include <string>
#include "BeatmapEnums.h"
#include "GameMode.h"

namespace osu {
    struct GeneralSettings {
        std::string AudioFilename;
        long long int AudioLeadIn{0};
        long long int PreviewTime{-1};
        char Countdown{1};
        SampleSets SampleSet{Normal};
        float StackLeniency{0.7};
        GameMode Mode{Standard};
        bool LetterboxInBreaks{false};
        bool UseSkinSprites{false};
        OverlayPositions OverlayPosition{NoChange};
        std::string SkinPreference;
        bool EpilepsyWarning{false};
        long long int CountdownOffset{0};
        bool SpecialStyle{false};
        bool WidescreenStoryboard{false};
        bool SamplesMatchPlaybackRate{false};
    };
    struct EditorSettings {
        std::string Bookmarks;
        float DistanceSpacing;
        float BeatDivisor;
        long long int GridSize;
        float TimelineZoom;
    };
    struct MetadataSettings {
        std::string Title;
        std::string TitleUnicode;
        std::string Artist;
        std::string ArtistUnicode;
        std::string Creator;
        std::string Version;
        std::string Source;
        std::string Tags;
        long long int BeatmapID{0};
        long long int BeatmapSetID{-1};
    };
    struct DifficultySettings {
        float HPDrainRate;
        float CircleSize;
        float OverallDifficulty;
        float ApproachRate;
        float SliderMultiplier;
        float SliderTickRate;
    };

    class BeatmapMetadata {
    public:
        GeneralSettings General;
        EditorSettings Editor;
        MetadataSettings Metadata;
        DifficultySettings Difficulty;
    };
}

#endif //OSU_LASER_C_BEATMAPMETADATA_H
