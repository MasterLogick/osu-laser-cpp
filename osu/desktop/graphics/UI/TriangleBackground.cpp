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

    const float MIN_VELOCITY = 1 / 2.0f;

    const float MAX_R_COLOR = 240 / 255.0f;

    const float MAX_G_COLOR = 120 / 255.0f;

    const float MAX_B_COLOR = 180 / 255.0f;

    const float MIN_R_COLOR = 220 / 255.0f;

    const float MIN_G_COLOR = 90 / 255.0f;

    const float MIN_B_COLOR = 150 / 255.0f;

    void fillEachWithRandomNumbers(float *array, int len, float minValue, float maxValue, int stride) {
        for (float *i = array; i < array + len; i += stride) {
            *i = minValue + (float) (rand()) / ((float) (RAND_MAX / (maxValue - minValue)));
        }
    }

    void fillEachWithRandomNumbers(float *array, int len, float maxValue, int stride) {
        fillEachWithRandomNumbers(array, len, 0, maxValue, stride);
    }

    void fillWithRandomNumbers(float *array, int len, float minValue, float maxValue) {
        fillEachWithRandomNumbers(array, len, minValue, maxValue, 1);
    }

    void fillWithRandomNumbers(float *array, int len, float maxValue) {
        fillEachWithRandomNumbers(array, len, 0, maxValue, 1);
    }


    TriangleBackground::TriangleBackground(int trianglesCount) {
        amount = trianglesCount;
        positions = new float[2 * amount];
        scale = new float[amount];
        color = new float[amount * 3];
        velocity = new float[amount];

    }

    void TriangleBackground::initialise() {
        fillEachWithRandomNumbers(positions, 2 * amount, (float) Graphics::mainScreen->getWidth(), 2);
        fillEachWithRandomNumbers(positions + 1, 2 * amount, (float) Graphics::mainScreen->getHeight(), 2);
        fillEachWithRandomNumbers(color, amount * 3, MIN_R_COLOR, MAX_R_COLOR, 3);
        fillEachWithRandomNumbers(color + 1, amount * 3, MIN_G_COLOR, MAX_G_COLOR, 3);
        fillEachWithRandomNumbers(color + 2, amount * 3, MIN_B_COLOR, MAX_B_COLOR, 3);
        fillWithRandomNumbers(scale, amount, MIN_SCALE, MAX_SCALE);
        fillWithRandomNumbers(velocity, amount, MIN_VELOCITY, MAX_VELOCITY);
//        for (int i = 0; i < amount * 2; ++i, ++i) {
//            std::cout << positions[i] << " " << positions[i + 1] << std::endl;
//        }
        glCreateVertexArrays(1, &vao);
        unsigned int vbos[BUFFERS_AMOUNT];
        glCreateBuffers(4, vbos);
        localPosVBO = vbos[0];
        globalPosVBO = vbos[1];
        colorVBO = vbos[2];
        scaleVBO = vbos[3];
        glNamedBufferData(localPosVBO, 6 * sizeof(float), vertices, GL_STATIC_DRAW);
        glNamedBufferData(globalPosVBO, amount * 2 * sizeof(float), positions, GL_STREAM_DRAW);
        glNamedBufferData(colorVBO, amount * 3 * sizeof(float), positions, GL_DYNAMIC_DRAW);
        glNamedBufferData(scaleVBO, amount * 1 * sizeof(float), scale, GL_DYNAMIC_DRAW);

        GLuint localPosLocation = Shader::triangleShader->getAttribLocation("localPos");
        GLuint globalPosLocation = Shader::triangleShader->getAttribLocation("globalPos");
        GLuint alphaLocation = Shader::triangleShader->getAttribLocation("alpha");
        GLuint scaleLocation = Shader::triangleShader->getAttribLocation("scale");
        GLuint offsets[] = {0, 0, 0, 0};
        GLuint strides[] = {2 * sizeof(float), 2 * sizeof(float), 3 * sizeof(float), 1 * sizeof(float)};
        GLuint locations[] = {localPosLocation, globalPosLocation, alphaLocation, scaleLocation};
        for (int i = 0; i < BUFFERS_AMOUNT; i++) {
            glEnableVertexArrayAttrib(vao, locations[i]);
            glVertexArrayAttribFormat(vao, locations[i], 2, GL_FLOAT, false, 0);
            glVertexArrayVertexBuffer(vao, locations[i], vbos[i], offsets[i], strides[i]);
        }

        glVertexArrayBindingDivisor(vao, globalPosLocation, 1);
        glVertexArrayBindingDivisor(vao, alphaLocation, 1);
        glVertexArrayBindingDivisor(vao, scaleLocation, 1);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, localPosVBO);
        glBindVertexArray(0);
    }

    void TriangleBackground::draw(int x, int y) {
        movePositions();
        Shader::triangleShader->bind();
        Shader::triangleShader->uniform("x", (float) x);
        Shader::triangleShader->uniform("y", (float) y);
        glBlendFunc(GL_ONE, GL_ZERO);
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
        glNamedBufferSubData(colorVBO, 0L, 3 * amount * sizeof(float), color);
        glNamedBufferSubData(scaleVBO, 0L, amount * sizeof(float), scale);
    }
}