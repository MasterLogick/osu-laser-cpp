//
// Created by MasterLogick on 5/7/20.
//

#ifndef OSU_LASER_C_POINT_H
#define OSU_LASER_C_POINT_H

namespace osu {
    struct Point {
    public:
        int x;
        int y;

        void interpolate(float t, Point &b);
    };

    struct PointF {
    public:
        float x;
        float y;
    };
}

#endif //OSU_LASER_C_POINT_H
