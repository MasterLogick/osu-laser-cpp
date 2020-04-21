//
// Created by MasterLogick on 2/4/20.
//
#include <cmath>
#include <thread>
#include <fstream>
#include <iostream>
#include <sstream>
#include "SOIL.h"
#include "Font.h"
#include "../opengl/Shader.h"

namespace osu {

    Font *Font::Exo2_0_Black = nullptr;
#ifdef COMPILE_WITH_ALL_FONTS
    Font *Font::Exo2_0_BlackItalic = nullptr;
    Font *Font::Exo2_0_Bold = nullptr;
    Font *Font::Exo2_0_BoldItalic = nullptr;
    Font *Font::Exo2_0_Light = nullptr;
    Font *Font::Exo2_0_LightItalic = nullptr;
    Font *Font::Exo2_0_Medium = nullptr;
    Font *Font::Exo2_0_MediumItalic = nullptr;
    Font *Font::Exo2_0_Regular = nullptr;
    Font *Font::Exo2_0_RegularItalic = nullptr;
    Font *Font::Exo2_0_SemiBold = nullptr;
    Font *Font::Exo2_0_SemiBoldItalic = nullptr;
    Font *Font::Noto_Basic = nullptr;
    Font *Font::Noto_CJK_Basic = nullptr;
    Font *Font::Noto_CJK_Compatibility = nullptr;
    Font *Font::Noto_Hangul = nullptr;
    Font *Font::osuFont = nullptr;
    Font *Font::Venera = nullptr;
    Font *Font::Venera_Light = nullptr;
    Font *Font::Venera_Medium = nullptr;
#endif
    bool Font::is = true;

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

    unsigned int charsToInt(const char *c) {
        return ((unsigned char) c[3] << 24) | ((unsigned char) c[2]) << 16 | ((unsigned char) c[1]) << 8 | ((unsigned char) c[0]);
    }

    unsigned short int charsToWord(const char *c) {
        return ((unsigned char) c[1]) << 8 | (unsigned char) c[0];
    }

    Font::Font(const char *name) {
        std::string fontBinData;
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        std::string filePath = std::string(FONTS_PATH) + name + ".bin";
        try {
            file.open(filePath);
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
        kerningPairsAmount = charsToInt(fileData) / FONT_KERNING_BLOCK_SIZE;
        kernings = new Kerning[kerningPairsAmount];
        saved = fileData;
        fileData += 4;
        for (int i = 0; saved + kerningPairsAmount * FONT_KERNING_BLOCK_SIZE > fileData; i++, fileData += FONT_KERNING_BLOCK_SIZE) {
            kernings[i].first = charsToInt(fileData);
            kernings[i].second = charsToInt(fileData + 4);
            kernings[i].size = (signed short) charsToWord(fileData + 8);
        }
#ifndef NDEBUG
        std::cout << "Font '" << info.fontName << "' metadata read" << std::endl;
#endif
    }

    void Font::initialise() {
        //--------Fonts loading block-----------
        std::thread thExo2_0_B = std::thread([]() -> void {
            Font::Exo2_0_Black = new Font("Exo2.0-Black");
        });
#ifdef COMPILE_WITH_ALL_FONTS
        std::thread thExo2_0_Bla = std::thread([]() -> void {
            Font::Exo2_0_BlackItalic = new Font("Exo2.0-BlackItalic");
        });
        std::thread thExo2_0_Bol = std::thread([]() -> void {
            Font::Exo2_0_Bold = new Font("Exo2.0-Bold");
        });
        std::thread thExo2_0_BolI = std::thread([]() -> void {
            Font::Exo2_0_BoldItalic = new Font("Exo2.0-BoldItalic");
        });
        std::thread thExo2_0_Lig = std::thread([]() -> void {
            Font::Exo2_0_Light = new Font("Exo2.0-Light");
        });
        std::thread thExo2_0_LigI = std::thread([]() -> void {
            Font::Exo2_0_LightItalic = new Font("Exo2.0-LightItalic");
        });
        std::thread thExo2_0_Med = std::thread([]() -> void {
            Font::Exo2_0_Medium = new Font("Exo2.0-Medium");
        });
        std::thread thExo2_0_MedI = std::thread([]() -> void {
            Font::Exo2_0_MediumItalic = new Font("Exo2.0-MediumItalic");
        });
        std::thread thExo2_0_Reg = std::thread([]() -> void {
            Font::Exo2_0_Regular = new Font("Exo2.0-Regular");
        });
        std::thread thExo2_0_RegI = std::thread([]() -> void {
            Font::Exo2_0_RegularItalic = new Font("Exo2.0-RegularItalic");
        });
        std::thread thExo2_0_Sem = std::thread([]() -> void {
            Font::Exo2_0_SemiBold = new Font("Exo2.0-SemiBold");
        });
        std::thread thExo2_0_SemI = std::thread([]() -> void {
            Font::Exo2_0_SemiBoldItalic = new Font("Exo2.0-SemiBoldItalic");
        });
        std::thread thNoto_Basic = std::thread([]() -> void {
            Font::Noto_Basic = new Font("Noto-Basic");
        });
        std::thread thNoto_CJK_B = std::thread([]() -> void {
            Font::Noto_CJK_Basic = new Font("Noto-CJK-Basic");
        });
        std::thread thNoto_CJK_C = std::thread([]() -> void {
            Font::Noto_CJK_Compatibility = new Font("Noto-CJK-Compatibility");
        });
        std::thread thNoto_Hangu = std::thread([]() -> void {
            Font::Noto_Hangul = new Font("Noto-Hangul");
        });
        std::thread thosuFont = std::thread([]() -> void {
            Font::osuFont = new Font("osuFont");
        });
        std::thread thVenera = std::thread([]() -> void {
            Font::Venera = new Font("Venera");
        });
        std::thread thVenera_Lig = std::thread([]() -> void {
            Font::Venera_Light = new Font("Venera-Light");
        });
        std::thread thVenera_Med = std::thread([]() -> void {
            Font::Venera_Medium = new Font("Venera-Medium");
        });
        thExo2_0_Bla.join();
        Font::Exo2_0_BlackItalic->loadTextures();
        thExo2_0_Bol.join();
        Font::Exo2_0_Bold->loadTextures();
        thExo2_0_BolI.join();
        Font::Exo2_0_BoldItalic->loadTextures();
        thExo2_0_Lig.join();
        Font::Exo2_0_Light->loadTextures();
        thExo2_0_LigI.join();
        Font::Exo2_0_LightItalic->loadTextures();
        thExo2_0_Med.join();
        Font::Exo2_0_Medium->loadTextures();
        thExo2_0_MedI.join();
        Font::Exo2_0_MediumItalic->loadTextures();
        thExo2_0_Reg.join();
        Font::Exo2_0_Regular->loadTextures();
        thExo2_0_RegI.join();
        Font::Exo2_0_RegularItalic->loadTextures();
        thExo2_0_Sem.join();
        Font::Exo2_0_SemiBold->loadTextures();
        thExo2_0_SemI.join();
        Font::Exo2_0_SemiBoldItalic->loadTextures();
        thNoto_Basic.join();
        Font::Noto_Basic->loadTextures();
        thNoto_CJK_B.join();
        Font::Noto_CJK_Basic->loadTextures();
        thNoto_CJK_C.join();
        Font::Noto_CJK_Compatibility->loadTextures();
        thNoto_Hangu.join();
        Font::Noto_Hangul->loadTextures();
        thosuFont.join();
        Font::osuFont->loadTextures();
        thVenera.join();
        Font::Venera->loadTextures();
        thVenera_Lig.join();
        Font::Venera_Light->loadTextures();
        thVenera_Med.join();
        Font::Venera_Medium->loadTextures();
#endif
        thExo2_0_B.join();
        Font::Exo2_0_Black->loadTextures();
    }

    void Font::loadTextures() {
        pages = new Page[common.pages];
        for (int i = 0; i < common.pages; ++i) {
            std::string name = std::string(FONTS_PATH) + pageNames[i];
            GLuint texture;
            glCreateTextures(GL_TEXTURE_2D, 1, &texture);
            int width, height, channels;
            unsigned char *data = SOIL_load_image(name.c_str(), &width, &height, &channels, 4);
            if (data) {
                glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
                glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
            } else {
                std::cerr << "texture loading error" << std::endl;
                exit(-1);
            }
            SOIL_free_image_data(data);
            pages[i].id = texture;
            pages[i].width = width;
            pages[i].height = height;
        }
        GLuint positionLocation = Shader::fontShader->getAttribLocation("position");
        GLuint texCordLocation = Shader::fontShader->getAttribLocation("texCord");
        glCreateVertexArrays(1, &vao);

        glCreateBuffers(1, &vbo);
        glNamedBufferData(vbo, 2 * 2 * 4 * sizeof(float), nullptr, GL_STREAM_DRAW);

        glEnableVertexArrayAttrib(vao, positionLocation);
        glVertexArrayAttribFormat(vao, positionLocation, 2, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(vao, positionLocation, vbo, 0, 2 * 2 * sizeof(float));

        glEnableVertexArrayAttrib(vao, texCordLocation);
        glVertexArrayAttribFormat(vao, texCordLocation, 2, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(vao, texCordLocation, vbo, 2 * sizeof(float), 2 * 2 * sizeof(float));
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
        float vertsData[16];
        for (int i = 0; i < len; ++i) {
            Char *c = binSearch(str[i]);
            float xoffest = carretGlobalPos == x ? 0 : c->xoffset * m;
            //
            //     4,5,6,7         0,1,2,3
            //     +---------------+
            //     | 3           2 |
            //     |               |
            //     |    (glyph)    |               (x,y,s,t)
            //     |               |
            //     |               |
            //     | 1           0 |
            //     +---------------+
            //     12,13,14,15     8,9,10,11
            //
            vertsData[12] = vertsData[4] = carretGlobalPos - xoffest;
            vertsData[8] = vertsData[0] = carretGlobalPos - xoffest + c->width * m;
            vertsData[13] = vertsData[9] = ceilLine - c->yoffset * m - c->height * m;
            vertsData[5] = vertsData[1] = ceilLine - c->yoffset * m;
            vertsData[14] = vertsData[6] = (float) c->x / pages[c->page].width;
            vertsData[10] = vertsData[2] = ((float) c->x + c->width) / pages[c->page].width;
            vertsData[7] = vertsData[3] = (float) c->y / pages[c->page].height;
            vertsData[15] = vertsData[11] = ((float) c->y + c->height) / pages[c->page].height;

            glBindTextureUnit(0, pages[c->page].id);
            glNamedBufferSubData(vbo, 0, 2 * 2 * 4 * sizeof(float), vertsData);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            carretGlobalPos += c->xadvance * m;
            if (Font::is)
                if (i < len - 1) {
                    Kerning *c = getKerning(str[i], str[i + 1]);
                    if (c->first == str[i] && c->second == str[i + 1]) {
                        carretGlobalPos -= c->size;
                    }
                }
        }
    }

    Kerning *Font::getKerning(wchar_t first, wchar_t second) {
        unsigned int left = 0, right = kerningPairsAmount;
        unsigned int middle;
        while (left < right) {
            middle = (left + right) / 2;
            if ((kernings + middle)->first < first) {
                left = middle + 1;
            } else {
                right = middle;
            }
        }
        right = kerningPairsAmount;
        while (left < right) {
            middle = (left + right) / 2;
            if ((kernings + middle)->second <= second && (kernings + middle)->first == first) {
                left = middle + 1;
            } else {
                right = middle;
            }
        }
        return kernings + right - 1;
    }
}