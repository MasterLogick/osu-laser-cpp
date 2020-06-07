//
// Created by MasterLogick on 2/13/20.
//
#include <fstream>
#include <iostream>
#include <sstream>
#include "OsuDatabase.h"

namespace osu {
    static size_t uleb128(char *src, unsigned long long int *num) {
        *num = 0;
        for (size_t shift = 0; true; shift++) {
            char b = *(src + shift);
            *num |= (b & 0x7f) << shift;
            if ((b & 0x80) == 0)
                return shift;
        }
    }

    void OsuDatabase::load(const char *path) {

//        std::string fontBinData;
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            file.open(path);
            /*std::stringstream sstream;
            sstream << file.rdbuf();
            file.close();
            fontBinData = sstream.str();*/
        }
        catch (std::ifstream::failure e) {
            std::cout << "Font bin file data reading error:" << e.what() << std::endl;
        }

//        const char *fileData = (fontBinData.c_str());

    }
}