//
// Created by MasterLogick on 10/14/20.
//

#include "GameScreenDrawable.h"
#include "../../opengl/UtillDrawer.h"
#include "../../UI/skin/SkinSprites.h"
#include "../../../interaction/Properties.h"
#include "../../Graphics.h"
#include "../../UI/Font.h"
#include <cmath>
#include <iostream>
#include <cmath>

#define GAME_SCREEN_DRAWING_LAYER -90
#define OSU_SCREEN_WIDTH 640
#define OSU_SCREEN_HEIGHT 480
#define OSU_PLAYFIELD_WIDTH 512
#define OSU_PLAYFIELD_HEIGHT 384
#define OSU_PLAYFIELD_XOFFSET (OSU_SCREEN_WIDTH - OSU_PLAYFIELD_WIDTH) / 2
#define OSU_PLAYFIELD_YOFFSET (OSU_SCREEN_HEIGHT - OSU_PLAYFIELD_HEIGHT) / 2
namespace osu {

    GameScreenDrawable::GameScreenDrawable(Beatmap *map) : beatmap(map), left(beatmap->hitObjects.begin()),
                                                           right(beatmap->hitObjects.begin()) {
        if (beatmap->metadata->Difficulty.ApproachRate < 5) {
            preempt = 1200 + 600 * (5 - beatmap->metadata->Difficulty.ApproachRate) / 5;
            fadeIn = 800 + 400 * (5 - beatmap->metadata->Difficulty.ApproachRate) / 5;
        } else if (beatmap->metadata->Difficulty.ApproachRate == 5) {
            preempt = 1200;
            fadeIn = 800;
        } else {
            preempt = 1200 - 750 * (beatmap->metadata->Difficulty.ApproachRate - 5) / 5;
            fadeIn = 800 - 500 * (beatmap->metadata->Difficulty.ApproachRate - 5) / 5;
        }
        Point arr[] = {{100, 100},
                       {200, 200},
                       {300, 100}};
        sp = SliderPath(arr, 3, 100 * M_PI, 0.1);
    }

    void GameScreenDrawable::initialise() {
        xAxisMultiplier = (float) Graphics::mainScreen->getWidth() / OSU_SCREEN_WIDTH;
        yAxisMultiplier = (float) Graphics::mainScreen->getHeight() / OSU_SCREEN_HEIGHT;
        diameterMultiplier = (xAxisMultiplier + yAxisMultiplier) / 2.0f;
        std::cout << beatmap->hitObjects.front()->time << std::endl;
    }

    void GameScreenDrawable::draw() {
        if (firstFrame) {
            startDraw();
            firstFrame = false;
        }
        std::chrono::time_point now = std::chrono::system_clock::now();
        beatmapTime =
                passedBeforePause + std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPause).count();
#ifndef NDEBUG
        std::wstring s = std::to_wstring(
                std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPause).count());
        float c[] = {0.5, 0.5, 0.5, 1};
        Font::Exo2_0_Black->draw(const_cast<wchar_t *>(s.c_str()), s.length(), 0, 100, 50, c);
        s = std::to_wstring(beatmapTime);
        Font::Exo2_0_Black->draw(const_cast<wchar_t *>(s.c_str()), s.length(), 0, 150, 50, c);
#endif
        while ((*left)->time < beatmapTime - preempt) { left++; }
        while (right != beatmap->hitObjects.end() && (*right)->time >= beatmapTime + postDraw) { right++; }
        std::for_each(beatmap->hitObjects.begin(), beatmap->hitObjects.end(), [this](Circle *c) {
            if (c->time - preempt <= beatmapTime && beatmapTime <= c->time + postDraw) {
                drawCircle(c);
            }
        });
        std::for_each(sp.mainSpline.begin(), sp.mainSpline.end(), [](Point p) {
            UtillDrawer::drawRectSprite(p.x, p.y, p.x + 20, p.y + 20, GAME_SCREEN_DRAWING_LAYER,
                                        SkinSprites::HitCircleOverlay, 1);
        });
    }

    void GameScreenDrawable::drawCircle(Circle *c) {
        float diameter = (54.4f - 4.48f * beatmap->metadata->Difficulty.CircleSize) * 2.0f * diameterMultiplier;
        float xPos = (c->pos.x + OSU_PLAYFIELD_XOFFSET) * xAxisMultiplier;
        float yPos = (OSU_PLAYFIELD_HEIGHT - c->pos.y + OSU_PLAYFIELD_YOFFSET) * yAxisMultiplier;
        UtillDrawer::drawRectSprite(xPos, yPos, xPos + diameter, yPos + diameter, GAME_SCREEN_DRAWING_LAYER,
                                    SkinSprites::HitCircleOverlay, (beatmapTime >= c->time + fadeIn - preempt) ? 1.0f :
                                                                   ((float) beatmapTime -
                                                                    ((float) c->time - (float) preempt)) /
                                                                   (float) fadeIn);
    }

    void GameScreenDrawable::drawSlider(Slider *s) {
        if (s->curveType == CurveType::CTBezier) {
            Point pts[s->curvePointsCount];
            std::memcpy(pts, s->curvePoints, s->curvePointsCount * sizeof(Point));
            for (int i = s->curvePointsCount - 1; i > 0; ++i) {
                for (int j = 0; j < i; ++j) {
//                    pts[j].x = pts[j]+
                }
            }
        }
    }

    void GameScreenDrawable::startDraw() {
        lastPause = std::chrono::system_clock::now();
        beatmapTime = std::min(
                beatmap->hitObjects.front()->time,/*beatmap->storyboard->commandContainers.front()->startTime*/
                0); //todo return commented fragment when storyboard will be written
        while ((*right)->time <= beatmapTime) { right++; }
        drawing = true;
    }

    void GameScreenDrawable::pauseDraw() {
        drawing = false;
        lastPause = std::chrono::system_clock::now();
    }

    void GameScreenDrawable::resumeDraw() {
        drawing = true;
    }

}