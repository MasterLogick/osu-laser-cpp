
//
// Created by MasterLogick on 2/5/20.
//



#include "PostProcessor.h"
#include "Shader.h"

namespace osu {

    static float quadVerticesData[] = {
            1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f
    };

    void PostProcessor::initialise(int width, int height) {
        this->width = width;
        this->height = height;
        //creating post processing framebuffer and texture
        glCreateFramebuffers(1, &postProcessingFramebuffer);
        glCreateTextures(GL_TEXTURE_2D, 1, &postProcessingdTexture);
        //configuring post processing texture
        glTextureParameteri(postProcessingdTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(postProcessingdTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage2D(postProcessingdTexture, 1, GL_RGB8, width, height);
        //texture binding and checking
        glNamedFramebufferTexture(postProcessingFramebuffer, GL_COLOR_ATTACHMENT0, postProcessingdTexture, 0);
        if (glCheckNamedFramebufferStatus(postProcessingFramebuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "PostProcessor framebuffer creating error "
                      << glCheckNamedFramebufferStatus(postProcessingFramebuffer, GL_FRAMEBUFFER) << std::endl;
        }
        //creating multi sampled framebuffer and texture
        glCreateFramebuffers(1, &multiSampledFramebuffer);
        glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &multiSampledTexture);
        //configuring multi sampled texture
        glTextureStorage2DMultisample(multiSampledTexture, msaaLevel, GL_RGBA8, width, height, GL_TRUE);
        glNamedFramebufferTexture(multiSampledFramebuffer, GL_COLOR_ATTACHMENT0, multiSampledTexture, 0);
        //creating renderbuffer
        glCreateRenderbuffers(1, &multiSampledRenderbuffer);
        //configuring and checking renderbuffer
        glNamedRenderbufferStorageMultisample(multiSampledRenderbuffer, msaaLevel, GL_DEPTH24_STENCIL8, width, height);
        glNamedFramebufferRenderbuffer(multiSampledFramebuffer, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                       multiSampledRenderbuffer);
        if (glCheckNamedFramebufferStatus(multiSampledFramebuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "MultiSampledRenderbuffer framebuffer creating error "
                      << glCheckNamedFramebufferStatus(multiSampledFramebuffer, GL_FRAMEBUFFER) << std::endl;
        }
        //creating vertex buffers and mapping memory for future writes
        glCreateVertexArrays(1, &quadVAO);
        glCreateBuffers(1, &quadVBO);

        GLuint positionLocation = Shader::postProcessorShader->getAttribLocation("position");
        GLuint texCordLocation = Shader::postProcessorShader->getAttribLocation("texCord");

        glNamedBufferData(quadVBO, 4 * 2 * 2 * sizeof(float), quadVerticesData, GL_STATIC_DRAW);

        glEnableVertexArrayAttrib(quadVAO, positionLocation);
        glVertexArrayAttribFormat(quadVAO, positionLocation, 2, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(quadVAO, positionLocation, quadVBO, 0, 2 * 2 * sizeof(float));

        glEnableVertexArrayAttrib(quadVAO, texCordLocation);
        glVertexArrayAttribFormat(quadVAO, texCordLocation, 2, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(quadVAO, texCordLocation, quadVBO, 2 * sizeof(float), 2 * 2 * sizeof(float));

        Shader::postProcessorShader->bind();
        Shader::postProcessorShader->uniform("screenTexture", 0);
    }

    void osu::PostProcessor::postProcess(GLuint destinationFramebuffer) {
        glBlitNamedFramebuffer(multiSampledFramebuffer, postProcessingFramebuffer, 0, 0, width, height, 0, 0, width,
                               height,
                               GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, destinationFramebuffer);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_BLEND);
        Shader::postProcessorShader->bind();
        glBindTextureUnit(0, postProcessingdTexture);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void PostProcessor::preparePostProcessor() {
        glBindFramebuffer(GL_FRAMEBUFFER, multiSampledFramebuffer);
    }

    PostProcessor::PostProcessor(GLuint msaaLevel) : msaaLevel(msaaLevel) {}
}