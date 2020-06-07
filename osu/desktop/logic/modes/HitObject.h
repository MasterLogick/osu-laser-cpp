//
// Created by MasterLogick on 5/26/20.
//

#ifndef OSU_LASER_CPP_HITOBJECT_H
#define OSU_LASER_CPP_HITOBJECT_H
namespace osu {
    enum HitObjectType {
        Circle = 0b0000'0001,
        Slider = 0b0000'0010,
        Spinner = 0b0000'1000,
        Hold = 0b1000'0000,
        NewCombo = 0b0000'0100,
        SkipColors = 0b0111'0000
    };

    class HitObject {
    public:
        int time;
        uint8_t type;
    };
}
#endif //OSU_LASER_CPP_HITOBJECT_H
