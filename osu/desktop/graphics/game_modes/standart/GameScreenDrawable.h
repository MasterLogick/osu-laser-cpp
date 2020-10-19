//
// Created by MasterLogick on 10/14/20.
//

#ifndef OSU_LASER_CPP_GAMESCREENDRAWABLE_H
#define OSU_LASER_CPP_GAMESCREENDRAWABLE_H

#include "../../../logic/beatmap/Beatmap.h"
#include "../../../logic/modes/HitObject.h"
#include <chrono>

namespace osu {
    class GameScreenDrawable {
        Beatmap *beatmap;

        std::vector<Circle *>::iterator left, right;
        std::chrono::time_point<std::chrono::system_clock> lastPause;
        int beatmapTime = 0;
        int passedBeforePause = 0;
        int fadeIn = 0;
        int preempt = 100;
        int postDraw = 10;
        bool drawing = false;
        bool firstFrame = true;
        float xAxisMultiplier;
        float yAxisMultiplier;
        float diameterMultiplier;
        float xHitOffset;
        float yHitOffset;

        void drawCircle(Circle *c);

        void drawSlider(Slider *s);

        void startDraw();

    public:

        GameScreenDrawable(Beatmap *map);

        void initialise();

        void draw();

        void pauseDraw();

        void resumeDraw();
    };
}

#endif //OSU_LASER_CPP_GAMESCREENDRAWABLE_H
