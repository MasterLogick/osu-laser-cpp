//
// Created by MasterLogick on 1/25/20.
//

#include <cstdlib>
#include <glad/glad.h>
#include <cmath>
#include "TriangleBackground.h"
#include "../Graphics.h"
#include "../opengl/Shader.h"

#define BUFFERS_AMOUNT 4
namespace osu {
    const float vertices[] = {
            (float) (-sqrt(3) / 2.0f), -1.0f / 2.0f,
            (float) (sqrt(3) / 2.0f), -1.0f / 2.0f,
            0.0f, 1.0f
    };
    const float MAX_SCALE = 100;

    const float MIN_SCALE = 5;

    const float MAX_VELOCITY = 3;

    const float MIN_VELOCITY = 2.0f;

    const float MAX_R_COLOR = 240 / 255.0f;

    const float MIN_R_COLOR = 100 / 255.0f;

    void fillEachWithRandomNumbers(std::mt19937 *rndDevice, float *array, int len, float minValue, float maxValue, int stride) {
        //todo use mt19937 instead of rand
        for (float *i = array; i < array + len; i += stride) {
            *i = minValue + (float) ((*rndDevice)()) / ((float) (rndDevice->max() / (maxValue - minValue)));
        }
    }

    void fillEachWithRandomNumbers(std::mt19937 *rndDevice, float *array, int len, float maxValue, int stride) {
        fillEachWithRandomNumbers(rndDevice, array, len, 0, maxValue, stride);
    }

    void fillWithRandomNumbers(std::mt19937 *rndDevice, float *array, int len, float minValue, float maxValue) {
        fillEachWithRandomNumbers(rndDevice, array, len, minValue, maxValue, 1);
    }

    void fillWithRandomNumbers(std::mt19937 *rndDevice, float *array, int len, float maxValue) {
        fillEachWithRandomNumbers(rndDevice, array, len, 0, maxValue, 1);
    }


    TriangleBackground::TriangleBackground(int trianglesCount) : rndDevice() {
        amount = trianglesCount;
        positions = new float[2 * amount];
        scale = new float[amount];
        color = new float[amount];
        velocity = new float[amount];

    }

    void TriangleBackground::initialise() {
        fillEachWithRandomNumbers(&rndDevice, positions, 2 * amount, (float) Graphics::mainScreen->getWidth(), 2);
        fillEachWithRandomNumbers(&rndDevice, positions + 1, 2 * amount, (float) Graphics::mainScreen->getHeight(), 2);
        fillWithRandomNumbers(&rndDevice, color, amount, MIN_R_COLOR, MAX_R_COLOR);
        fillWithRandomNumbers(&rndDevice, scale, amount, MIN_SCALE, MAX_SCALE);
        fillWithRandomNumbers(&rndDevice, velocity, amount, MIN_VELOCITY, MAX_VELOCITY);

        glCreateVertexArrays(1, &vao);
        unsigned int vbos[BUFFERS_AMOUNT];
        glCreateBuffers(4, vbos);
        localPosVBO = vbos[0];
        globalPosVBO = vbos[1];
        colorVBO = vbos[2];
        scaleVBO = vbos[3];
        glNamedBufferData(localPosVBO, 6 * sizeof(float), vertices, GL_STATIC_DRAW);
        glNamedBufferData(globalPosVBO, amount * 2 * sizeof(float), positions, GL_STREAM_DRAW);
        glNamedBufferData(colorVBO, amount * 1 * sizeof(float), color, GL_DYNAMIC_DRAW);
        glNamedBufferData(scaleVBO, amount * 1 * sizeof(float), scale, GL_DYNAMIC_DRAW);

        GLuint localPosLocation = Shader::triangleShader->getAttribLocation("localPos");
        GLuint globalPosLocation = Shader::triangleShader->getAttribLocation("globalPos");
        GLuint alphaLocation = Shader::triangleShader->getAttribLocation("alpha");
        GLuint scaleLocation = Shader::triangleShader->getAttribLocation("scale");
        GLuint offsets[] = {0, 0, 0, 0};
        GLuint strides[] = {2 * sizeof(float), 2 * sizeof(float), 1 * sizeof(float), 1 * sizeof(float)};
        GLuint locations[] = {localPosLocation, globalPosLocation, alphaLocation, scaleLocation};
        GLuint sizes[] = {2, 2, 1, 1};
        for (int i = 0; i < BUFFERS_AMOUNT; i++) {
            glEnableVertexArrayAttrib(vao, locations[i]);
            glVertexArrayAttribFormat(vao, locations[i], sizes[i], GL_FLOAT, false, 0);
            glVertexArrayVertexBuffer(vao, locations[i], vbos[i], offsets[i], strides[i]);
        }

        glVertexArrayBindingDivisor(vao, globalPosLocation, 1);
        glVertexArrayBindingDivisor(vao, alphaLocation, 1);
        glVertexArrayBindingDivisor(vao, scaleLocation, 1);
    }

    void TriangleBackground::draw(int x, int y) {
        movePositions();
        Shader::triangleShader->bind();
        Shader::triangleShader->uniform("x", (float) x);
        Shader::triangleShader->uniform("y", (float) y);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, amount);
    }

    void TriangleBackground::movePositions() {
        for (int i = 0; i < amount; ++i) {
            if ((positions[2 * i + 1] += velocity[i]) > Graphics::mainScreen->getHeight() + MAX_SCALE) {
                positions[2 * i] = (float) (rand()) /
                                   ((float) (RAND_MAX / (Graphics::mainScreen->getWidth())));
                positions[2 * i + 1] = -MAX_SCALE;
                velocity[i] = (float) (rand()) / ((float) (RAND_MAX / (MAX_VELOCITY)));
            }
        }
        glNamedBufferSubData(globalPosVBO, 0L, 2 * amount * sizeof(float), positions);
        glNamedBufferSubData(colorVBO, 0L, amount * sizeof(float), color);
        glNamedBufferSubData(scaleVBO, 0L, amount * sizeof(float), scale);
    }
}