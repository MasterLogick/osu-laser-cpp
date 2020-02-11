//
// Created by Masterlogick on 2/5/20.
//

#include "PostProcessor.h"
#include "Shader.h"

namespace osu {

    void osu::PostProcessor::postProcess(GLuint destinationFramebuffer) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, multiSampledFramebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFramebuffer);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, destinationFramebuffer);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_BLEND);
        Shader::postProcessorShader->bind();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, postProcessingdTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void PostProcessor::preparePostProcessor() {
        glBindFramebuffer(GL_FRAMEBUFFER, multiSampledFramebuffer);
    }

    void PostProcessor::initialise(int width, int height) {
        this->width = width;
        this->height = height;
        //creating post processing framebuffer and attachments
        glGenFramebuffers(1, &postProcessingFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFramebuffer);

        glGenTextures(1, &postProcessingdTexture);
        glBindTexture(GL_TEXTURE_2D, postProcessingdTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_INT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingdTexture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "PostProcessor framebuffer creating error " << glCheckFramebufferStatus(GL_FRAMEBUFFER)
                      << std::endl;
        }

        //creating multi sampled framebuffer and attachments
        glGenFramebuffers(1, &multiSampledFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, multiSampledFramebuffer);

        glGenTextures(1, &multiSampledTexture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multiSampledTexture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaLevel, GL_RGBA, width,
                                height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multiSampledTexture, 0);

        glGenRenderbuffers(1, &multiSampledRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, multiSampledRenderbuffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaLevel, GL_DEPTH24_STENCIL8,
                                         width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, multiSampledRenderbuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "MultiSampledRenderbuffer framebuffer creating error " << glCheckFramebufferStatus(GL_FRAMEBUFFER)
                      << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenVertexArrays(1, &quadVAO);
        glBindVertexArray(quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 2 * 2 * sizeof(float), quadVerticesData, GL_STATIC_DRAW);
        GLuint positionLocation = Shader::postProcessorShader->getAttribLocation("position");
        GLuint texCordLocation = Shader::postProcessorShader->getAttribLocation("texCord");
        glEnableVertexAttribArray(positionLocation);
        glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 2 * 2 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(texCordLocation);
        glVertexAttribPointer(texCordLocation, 2, GL_FLOAT, GL_FALSE, 2 * 2 * sizeof(float), (void *) (2 * sizeof(float)));
        Shader::postProcessorShader->bind();
        Shader::postProcessorShader->uniform("screenTexture", 0);
    }

    PostProcessor::PostProcessor(GLuint msaaLevel) {
        this->msaaLevel = msaaLevel;
    }
}
/*
//
// Created by Masterlogick on 2/5/20.
//



#include "PostProcessor.h"
#include "Shader.h"

namespace osu {

    void osu::PostProcessor::postProcess(GLuint destinationFramebuffer) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, multiSampledFramebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFramebuffer);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, destinationFramebuffer);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_BLEND);
        Shader::postProcessorShader->bind();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, postProcessingdTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void PostProcessor::preparePostProcessor() {
        glBindFramebuffer(GL_FRAMEBUFFER, multiSampledFramebuffer);
    }

    void PostProcessor::initialise(int width, int height) {
        this->width = width;
        this->height = height;
        //creating post processing framebuffer and attachments
        glCreateFramebuffers(1, &postProcessingFramebuffer);
//        glGenFramebuffers(1, &postProcessingFramebuffer);
//        glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFramebuffer);

        glCreateTextures(GL_TEXTURE_2D, 1, &postProcessingdTexture);
        glTextureParameteri(postProcessingdTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(postProcessingdTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage2D(postProcessingdTexture, 1, GL_RGB8, width, height);
//        glGenTextures(1, &postProcessingdTexture);
//        glBindTexture(GL_TEXTURE_2D, postProcessingdTexture);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_INT, nullptr);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glBindTexture(GL_TEXTURE_2D, 0);
        glNamedFramebufferTexture(postProcessingFramebuffer, GL_COLOR_ATTACHMENT0, postProcessingdTexture, 0);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingdTexture, 0);

        if (glCheckNamedFramebufferStatus(GL_FRAMEBUFFER, postProcessingFramebuffer) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "PostProcessor framebuffer creating error "
                      << glCheckNamedFramebufferStatus(GL_FRAMEBUFFER, postProcessingFramebuffer) << std::endl;
        }

        //creating multi sampled framebuffer and attachments
        glGenFramebuffers(1, &multiSampledFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, multiSampledFramebuffer);

        glGenTextures(1, &multiSampledTexture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multiSampledTexture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaLevel, GL_RGBA, width,
                                height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multiSampledTexture, 0);

        glGenRenderbuffers(1, &multiSampledRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, multiSampledRenderbuffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaLevel, GL_DEPTH24_STENCIL8,
                                         width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, multiSampledRenderbuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "MultiSampledRenderbuffer framebuffer creating error " << glCheckFramebufferStatus(GL_FRAMEBUFFER)
                      << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenVertexArrays(1, &quadVAO);
        glBindVertexArray(quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 2 * 2 * sizeof(float), quadVerticesData, GL_STATIC_DRAW);
        GLuint positionLocation = Shader::postProcessorShader->getAttribLocation("position");
        GLuint texCordLocation = Shader::postProcessorShader->getAttribLocation("texCord");
        glEnableVertexAttribArray(positionLocation);
        glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 2 * 2 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(texCordLocation);
        glVertexAttribPointer(texCordLocation, 2, GL_FLOAT, GL_FALSE, 2 * 2 * sizeof(float), (void *) (2 * sizeof(float)));
        Shader::postProcessorShader->bind();
        Shader::postProcessorShader->uniform("screenTexture", 0);
    }

    PostProcessor::PostProcessor(GLuint msaaLevel) {
        this->msaaLevel = msaaLevel;
    }
}
 * */