//
// Created by MasterLogick on 1/25/20.
//

#ifndef OSU_LASER_C_POSTPROCESSOR_H
#define OSU_LASER_C_POSTPROCESSOR_H

#include "glad/glad.h"
#include "../Graphics.h"

namespace osu {
    const float quadVerticesData[] = {
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
    };

    class PostProcessor {
    private:
        int width;
        int height;
        GLuint multiSampledFramebuffer;
        GLuint multiSampledTexture;
        GLuint multiSampledRenderbuffer;
        GLuint postProcessingFramebuffer;
        GLuint postProcessingdTexture;
        GLuint quadVAO;
        GLuint quadVBO;
        GLuint msaaLevel;
    public:
        explicit PostProcessor(GLuint msaaLevel);

        void initialise(int width, int height);

        void preparePostProcessor();

        void postProcess(GLuint destinationFramebuffer);
    };
}

#endif //OSU_LASER_C_POSTPROCESSOR_H
