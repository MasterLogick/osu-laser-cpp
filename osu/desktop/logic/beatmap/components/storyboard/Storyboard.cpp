//
// Created by MasterLogick on 9/5/20.
//

#include "Storyboard.h"

namespace osu {
    void Storyboard::cacheSprites() {
        for (int i = 0, j = events.size(); i < j; ++i) {
            Event *e = events[i];
            if (e->eventType == EventType::ETAnimation) {
                Animation *anim = static_cast<Animation *>(e);
                path p = dir / anim->file;
                std::string &stem = const_cast<std::string &>((p.parent_path() / p.stem()).string());
                std::string &extension = const_cast<std::string &>(p.extension().string());
                Texture *textures = new Texture[anim->frameCount];
                for (int k = 0; k < anim->frameCount; ++k) {
                    textures[k] = Texture(stem + std::to_string(k) + extension);
                }
                anim->textureArray = textures;
            } else if (e->eventType == EventType::ETBackground) {

            }
        }
    }
}