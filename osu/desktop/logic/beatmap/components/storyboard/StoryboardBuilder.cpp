//
// Created by MasterLogick on 6/18/20.
//

#include "StoryboardBuilder.h"
#include "Background.h"
#include "Timeline.h"
#include "../../../../../test/ExecutionTimer.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <algorithm>

namespace osu {
    StoryboardBuilder::StoryboardBuilder() {

    }

    StoryboardBuilder::StoryboardBuilder(path dir) : dir(dir) {}

    void StoryboardBuilder::addEvent(Event *event) {
        events.push_back(event);
        if (isDrawable(event->eventType)) {//todo replace with DrawableEvent::loadTextures call
            DrawableEvent *e = static_cast<DrawableEvent *>(event);
            if (e->eventType == EventType::ETBackground) {
                std::string p = (dir / static_cast<Background *>(e)->path).string();
                auto iterator = std::find_if(texturePaths.begin(), texturePaths.end(),
                                             [p](PreCachedImage &ref) -> bool {
                                                 return ref.framesPaths.front() == p;
                                             });
                if (iterator == texturePaths.end()) {
                    PreCachedImage pImage;
                    pImage.framesPaths.push_back(p);
                    pImage.events.push_back(e);
                    texturePaths.push_back(pImage);
                } else {
                    (*iterator).events.push_back(e);
                }
            } else if (e->eventType == EventType::ETAnimation) {
                Animation *anim = static_cast<Animation *>(e);
                path p = dir / anim->path;
                std::string stem = const_cast<std::string &>((p.parent_path() / p.stem()).string());
                std::string extension = const_cast<std::string &>(p.extension().string());
                std::string zeroFramePath = stem + std::to_string(0) + extension;
                auto iterator = std::find_if(texturePaths.begin(), texturePaths.end(),
                                             [zeroFramePath](PreCachedImage &ref) -> bool {
                                                 return ref.framesPaths.front() == zeroFramePath;
                                             });
                if (iterator == texturePaths.end()) {
                    PreCachedImage pImage;
                    pImage.framesPaths.reserve(anim->frameCount);
                    pImage.framesPaths.push_back(zeroFramePath);
                    for (int k = 1; k < anim->frameCount; ++k) {
                        pImage.framesPaths.push_back(stem + std::to_string(k) + extension);
                    }
                    pImage.events.push_back(e);
                    texturePaths.push_back(pImage);
                } else {
                    PreCachedImage &pImage = *iterator;
                    pImage.framesPaths.reserve(anim->frameCount);
                    for (int k = 0; k < anim->frameCount; ++k) {
                        pImage.framesPaths[k] = stem + std::to_string(k) + extension;
                    }
                    pImage.events.push_back(e);
                }
            } else if (e->eventType == EventType::ETSprite) {
                std::string p = (dir / static_cast<Sprite *>(e)->path).string();
                auto iterator = std::find_if(texturePaths.begin(), texturePaths.end(),
                                             [p](PreCachedImage &ref) -> bool {
                                                 return ref.framesPaths.front() == p;
                                             });
                if (iterator == texturePaths.end()) {
                    PreCachedImage pImage;
                    pImage.framesPaths.push_back(p);
                    pImage.events.push_back(e);
                    texturePaths.push_back(pImage);
                } else {
                    (*iterator).events.push_back(e);
                }
            }
        }
    }

    void StoryboardBuilder::addEventContainer(CompoundCommand *container) {
//        commandContainers.push_back(container);
//        timeline.insert(container);
    }

    void StoryboardBuilder::setRootPath(path dir) {
        this->dir = dir;
    }

    Storyboard *StoryboardBuilder::build() {
        std::sort(events.begin(), events.end(),
                  [](Event *e1, Event *e2) -> bool {
                      return e1->startTime < e2->startTime;
                  });
//        std::sort(commandContainers.begin(), commandContainers.end(),
//                  [](CompoundCommand *c1, CompoundCommand *c2) -> bool {
//                      return c1->startTime < c2->startTime;
//                  });
//        timeline.pack();
#ifndef NDEBUG
        std::cout << "Storyboard summary:" << std::endl;
        std::cout << "Events: " << events.size() << std::endl;
//        std::cout << "Time points: " << timeline.size() << std::endl;
        std::cout << "Textures: " << texturePaths.size() << std::endl;
#endif
        for (PreCachedImage pImage:texturePaths) {
            std::vector<std::string> &paths = pImage.framesPaths;
            Texture *textures = new Texture[paths.size()];
            for (int i = 0; i < paths.size(); ++i) {
                textures[i] = Texture(paths[i].c_str());
            }
            for (DrawableEvent *event:pImage.events) {
                event->texture = textures;
            }
        }
//        timeline.printTimestampsInfo();
        return nullptr;
    }

    path StoryboardBuilder::getRootPath() {
        return dir;
    }
}