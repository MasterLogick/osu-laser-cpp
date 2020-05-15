//
// Created by Masterlogick on 4/21/20.
//

#ifndef OSU_LASER_C_BEATMAPMETADATA_H
#define OSU_LASER_C_BEATMAPMETADATA_H

#include <cstdlib>
#include "BeatmapEnums.h"
#include "GameMode.h"

namespace osu {
    struct GeneralSettings {
        char *AudioFilename{nullptr};
        long long int AudioLeadIn{0};
        long long int PreviewTime{-1};
        char Countdown{1};
        SampleSets SampleSet{Normal};
        float StackLeniency{0.7};
        GameMode Mode{Standard};
        bool LetterboxInBreaks{false};
        bool UseSkinSprites{false};
        OverlayPositions OverlayPosition{NoChange};
        char *SkinPreference{nullptr};
        bool EpilepsyWarning{false};
        long long int CountdownOffset{0};
        bool SpecialStyle{false};
        bool WidescreenStoryboard{false};
        bool SamplesMatchPlaybackRate{false};
    };
    struct EditorSettings {
        char *Bookmarks{nullptr};
        float DistanceSpacing;
        float BeatDivisor;
        long long int GridSize;
        float TimelineZoom;
    };
    struct MetadataSettings {
        char *Title;
        char *TitleUnicode;
        char *Artist;
        char *ArtistUnicode;
        char *Creator;
        char *Version;
        char *Source;
        char *Tags;
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
