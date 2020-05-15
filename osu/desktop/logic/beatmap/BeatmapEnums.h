//
// Created by Masterlogick on 4/23/20.
//

#ifndef OSU_LASER_C_BEATMAPENUMS_H
#define OSU_LASER_C_BEATMAPENUMS_H

namespace osu {
    enum TimeSignatures {
        Triple = 3,
        Quadruple = 4
    };
    enum SampleSets {
        Normal = 1, Soft = 2, Drum = 3
    };
    enum OverlayPositions {
        NoChange, Below, Above
    };
    enum EventType {
        Background = 0, Video = 1, Break = 2
    };
}
#endif //OSU_LASER_C_BEATMAPENUMS_H
