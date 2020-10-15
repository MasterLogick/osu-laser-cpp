//
// Created by MasterLogick on 10/14/20.
//

#include "GameScreenDrawable.h"
#include "../../opengl/UtillDrawer.h"
#include "../../UI/skin/SkinSprites.h"
#include <cmath>
#include <iostream>

#define GAME_SCREEN_DRAWING_LAYER -90
namespace osu {

    GameScreenDrawable::GameScreenDrawable(Beatmap *map) : beatmap(map), left(beatmap->hitObjects.begin()),
                                                           right(beatmap->hitObjects.begin()), preempt(100) {}

    void GameScreenDrawable::initialise() {

    }

    void GameScreenDrawable::draw() {
        if (firstFrame) {
            startDraw();
            firstFrame = false;
        }
        UtillDrawer::drawRectSprite(0, 0, 100, 100, GAME_SCREEN_DRAWING_LAYER, SkinSprites::CircleSprite);
        std::chrono::time_point now = std::chrono::system_clock::now();
        beatmapTime += std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDraw).count();
        std::cout << std::distance(left, beatmap->hitObjects.begin()) << std::endl;
//        while ((*left)->time < beatmapTime - preempt) { left++; }
//        while (right != beatmap->hitObjects.end() && (*right)->time >= beatmapTime + postDraw) { right++; }
        std::for_each(left, right, [this](Circle *c) {
            if (c->time - preempt >= beatmapTime && c->time + postDraw <= beatmapTime) {
                drawCircle(c);
            }
        });

        std::cout << "draw tick " << std::distance(left, beatmap->hitObjects.begin()) << " "
                  << std::distance(right, beatmap->hitObjects.begin()) << std::endl;
    }

    void GameScreenDrawable::drawCircle(Circle *c) {
        std::cout << "draw circle" << std::endl;
        int diameter = (54.4 - 4.48 * beatmap->metadata->Difficulty.CircleSize) * 2;
        UtillDrawer::drawRectSprite(c->pos.x, c->pos.y, c->pos.x + diameter, c->pos.y + diameter,
                                    GAME_SCREEN_DRAWING_LAYER, SkinSprites::CircleSprite);
    }

    void GameScreenDrawable::startDraw() {
        lastDraw = std::chrono::system_clock::now();
        beatmapTime = std::min(
                beatmap->hitObjects.front()->time,/*beatmap->storyboard->commandContainers.front()->startTime*/
                0); //todo return commented fragment when storyboard will be written
        while ((*right)->time <= beatmapTime) { right++; }
        drawing = true;
    }

    void GameScreenDrawable::pauseDraw() {
        drawing = false;
    }

    void GameScreenDrawable::resumeDraw() {
        drawing = true;
    }

}