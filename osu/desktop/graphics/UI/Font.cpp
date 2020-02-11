//
// Created by MasterLogick on 2/4/20.
//

#include "SOIL.h"
#include "Font.h"
#include "../opengl/Shader.h"

namespace osu {
    static const unsigned int indices[] = {
            3, 1, 2,                        //23
            1, 0, 2                         //01
    };

    Char *Font::binSearch(wchar_t id) {
        unsigned int left = 0, right = charactersCount;
        unsigned int middle;
        while (left + 1 < right) {
            middle = (left + right) / 2;
            if ((chars + middle)->id <= id) {
                left = middle;
            } else {
                right = middle;
            }
        }
        middle = (left + right) / 2;
        return chars + middle;
    }

    Font *Font::Exo2_0_Black = nullptr;

    unsigned int charsToInt(const char *c) {
        return (unsigned char) c[3] << 24 | (unsigned char) c[2] << 16 | (unsigned char) c[1] << 8 | (unsigned char) c[0];
    }

    unsigned int charsToWord(const char *c) {
        return (unsigned char) c[1] << 8 | (unsigned char) c[0];
    }

    Font::Font(const char *name) {
        std::string fontBinData;
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        std::string vertexPath = std::string(FONTS_PATH) + name + ".bin";
        try {
            file.open(vertexPath);
            std::stringstream sstream;
            sstream << file.rdbuf();
            file.close();
            fontBinData = sstream.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "Font bin file data reading error:" << e.what() << std::endl;
        }
        const char *fileData = (fontBinData.c_str());
        int version = charsToInt(fileData);
        if (version != 0x03464d42) {
            std::cout << "error unsupported filetype" << std::endl;
            throw std::runtime_error("Unsupported font type");
        }
        fileData += 9;
        info.fontSize = charsToWord(fileData);
        info.bitfield = *(fileData + 2);
        info.charset = *(fileData + 3);
        info.stretchH = charsToWord(fileData + 4);
        info.aa = *(fileData + 6);
        info.paddingUp = *(fileData + 7);
        info.paddingRight = *(fileData + 8);
        info.paddingDown = *(fileData + 9);
        info.paddingLeft = *(fileData + 10);
        info.spacingHoriz = *(fileData + 11);
        info.spacingVert = *(fileData + 12);
        info.fontName = std::string(fileData + 14);
        fileData += 15 + info.fontName.size() + 5;
        if (~info.bitfield & FONT_UNICODE) {
            //todo err
        }
        common.lineHeight = charsToWord(fileData + 0);
        common.base = charsToWord(fileData + 2);
        common.scaleW = charsToWord(fileData + 4);
        common.scaleH = charsToWord(fileData + 6);
        common.pages = charsToWord(fileData + 8);
        common.bitField = *(fileData + 10);
        common.alphaChnl = *(fileData + 11);
        common.redChnl = *(fileData + 12);
        common.greenChnl = *(fileData + 13);
        common.blueChnl = *(fileData + 14);
        fileData += 15 + 5;
        pageNames = new std::string[common.pages];
        for (int i = 0; i < common.pages; ++i) {
            pageNames[i] = std::string(fileData);
            fileData += pageNames[i].size() + 1;
        }
        charactersCount = charsToInt(&fileData[1]) / FONT_CHAR_PACKET_SIZE;
        chars = new Char[charactersCount];
        fileData += 5;
        const char *saved = fileData;
        for (Char *c = chars; fileData < saved + charactersCount * FONT_CHAR_PACKET_SIZE; fileData += FONT_CHAR_PACKET_SIZE, c++) {
            c->id = charsToInt(fileData);
            c->x = charsToWord(fileData + 4);
            c->y = charsToWord(fileData + 6);
            c->width = charsToWord(fileData + 8);
            c->height = charsToWord(fileData + 10);
            c->xoffset = charsToWord(fileData + 12);
            c->yoffset = charsToWord(fileData + 14);
            c->xadvance = charsToWord(fileData + 16);
            c->page = (unsigned char) fileData[18];
            c->chnl = (unsigned char) fileData[19];
        }
        fileData++;
        unsigned int kerningPairsBlockSize = charsToInt(fileData);
        saved = fileData;
        while (saved + kerningPairsBlockSize > fileData) {
            char32_t first = charsToInt(fileData);
            char32_t second = charsToInt(fileData + 4);
            int amount = charsToWord(fileData + 8);
            kerningPairs[first][second] = amount;
            fileData += FONT_KERNING_BLOCK_SIZE;
        }
#ifndef NDEBUG
        std::cout << "Font '" << info.fontName << "' metadata read" << std::endl;
#endif
    }

    void Font::initialise() {
        //--------Fonts loading block begin-----------
        Font::Exo2_0_Black = new Font("Exo2.0-Black");
        //--------Fonts loading block end-------------
        Font::Exo2_0_Black->loadTextures();
    }

    void Font::loadTextures() {
        pages = new Page[common.pages];
        for (int i = 0; i < common.pages; ++i) {
            std::string name = std::string(FONTS_PATH) + pageNames[i];
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            int width, height, chanels;
            unsigned char *data = SOIL_load_image(name.c_str(), &width, &height, &chanels, 4);
            if (data) {
                glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            SOIL_free_image_data(data);
            glBindTexture(GL_TEXTURE_2D, 0);
            pages[i].id = texture;
            pages[i].width = width;
            pages[i].height = height;
#ifndef NDEBUG
            std::cout << "Texture '" << pageNames[i] << "' with width " << width << ", height " << height << " and channels amount "
                      << chanels << " loaded as texture " << texture << std::endl;
#endif
        }
        float data[16];
        GLuint positionLocation = Shader::fontShader->getAttribLocation("position");
        GLuint texCordLocation = Shader::fontShader->getAttribLocation("texCord");
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(float), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(positionLocation);
        glEnableVertexAttribArray(texCordLocation);
        glBufferData(GL_ARRAY_BUFFER, 2 * 2 * 4 * sizeof(float), data, GL_STREAM_DRAW);
        glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        glVertexAttribPointer(texCordLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        delete[] pageNames;
    }

    void Font::draw(wchar_t *str, int len, int x, int y, int size, float *color) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        float m = (float) size / common.lineHeight;
        float ceilLine = y + size;
        float carretGlobalPos = x;
        Shader::fontShader->bind();
        Shader::fontShader->uniform("textureSampler", 0);
        Shader::fontShader->uniform4("Color", color);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        float vertsData[16];
        for (int i = 0; i < len; ++i) {
            Char *c = binSearch(str[i]);
            float xoffest = carretGlobalPos == x ? 0 : c->xoffset * m;
            /*      0,1,2,3         4,5,6,7
             *      +---------------+
             *      | 2           3 |
             *      |               |
             *      |    (glyph)    |               (x,y,s,t)
             *      |               |
             *      |               |
             *      | 0           1 |
             *      +---------------+
             *      8,9,10,11       12,13,14,15
             */
            vertsData[8] = vertsData[0] = carretGlobalPos - xoffest;
            vertsData[12] = vertsData[4] = carretGlobalPos - xoffest + c->width * m;
            vertsData[13] = vertsData[9] = ceilLine - c->yoffset * m - c->height * m;
            vertsData[5] = vertsData[1] = ceilLine - c->yoffset * m;
            vertsData[10] = vertsData[2] = (float) c->x / pages[c->page].width;
            vertsData[14] = vertsData[6] = ((float) c->x + c->width) / pages[c->page].width;
            vertsData[7] = vertsData[3] = (float) c->y / pages[c->page].height;
            vertsData[15] = vertsData[11] = ((float) c->y + c->height) / pages[c->page].height;
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, pages[c->page].id);
            glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * 2 * 4 * sizeof(float), vertsData);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            carretGlobalPos += c->xadvance * m;
            if (i != len - 1) {
                auto iter = kerningPairs[str[i]].find(str[i + 1]);
                if (iter != kerningPairs[str[i]].end()) {
                    carretGlobalPos += iter->second * m;
                }
            }
        }
    }
}