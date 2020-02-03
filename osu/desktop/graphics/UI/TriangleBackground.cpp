//
// Created by user on 1/25/20.
//

#include <cstdlib>
#include <glad/glad.h>
#include <cmath>
#include "TriangleBackground.h"
#include "../Graphics.h"
#include "../opengl/Shader.h"

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
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        unsigned int vbos[4];
        glGenBuffers(4, vbos);
        localPosVBO = vbos[0];
        globalPosVBO = vbos[1];
        colorVBO = vbos[2];
        scaleVBO = vbos[3];
        GLuint localPosLoc = Shader::triangleShader->getAttribLocation("localPos");
        GLuint globalPosLoc = Shader::triangleShader->getAttribLocation("globalPos");
        GLuint alphaLoc = Shader::triangleShader->getAttribLocation("alpha");
        GLuint scaleLoc = Shader::triangleShader->getAttribLocation("scale");

        glBindBuffer(GL_ARRAY_BUFFER, localPosVBO);
        glEnableVertexAttribArray(localPosLoc);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(localPosLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);

        glBindBuffer(GL_ARRAY_BUFFER, globalPosVBO);
        glEnableVertexAttribArray(globalPosLoc);
        glBufferData(GL_ARRAY_BUFFER, 2 * amount * sizeof(float), positions, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(globalPosLoc, 2, GL_FLOAT, false, 2 * sizeof(float), (void *) 0);

        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, 3 * amount * sizeof(float), color, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(alphaLoc);
        glVertexAttribPointer(alphaLoc, 3, GL_FLOAT, false, 3 * sizeof(float), (void *) 0);

        glBindBuffer(GL_ARRAY_BUFFER, scaleVBO);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(float), scale, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(scaleLoc);
        glVertexAttribPointer(scaleLoc, 1, GL_FLOAT, false, 1 * sizeof(float), (void *) 0);

        glVertexAttribDivisor(globalPosLoc, 1);
        glVertexAttribDivisor(alphaLoc, 1);
        glVertexAttribDivisor(scaleLoc, 1);
        glBindBuffer(GL_ARRAY_BUFFER, localPosVBO);
        glBindVertexArray(0);
    }

    void TriangleBackground::draw(int x, int y) {
        movePositions();
        Shader::triangleShader->bind();
        Shader::triangleShader->uniform("x", (float) x);
        Shader::triangleShader->uniform("y", (float) y);
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
        glBindBuffer(GL_ARRAY_BUFFER, globalPosVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0L, 2 * amount * sizeof(float), positions);
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0L, 3 * amount * sizeof(float), color);
        glBindBuffer(GL_ARRAY_BUFFER, scaleVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0L, amount * sizeof(float), scale);
        glBindBuffer(GL_ARRAY_BUFFER, localPosVBO);
    }
}