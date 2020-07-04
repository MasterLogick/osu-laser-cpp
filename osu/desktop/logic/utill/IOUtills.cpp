//
// Created by MasterLogick on 7/3/20.
//

#include "IOUtills.h"
#include <iostream>

namespace osu {
    void readLineMultiplatform(std::istream &stream, std::string &line) {
        if (stream.bad() || stream.fail()) {
            //todo through exception
            return;
        }
        std::cout << (stream.rdstate() == std::ios::goodbit) << std::endl;
        line.clear();
        char c;
        while (stream.eof() || (c = stream.get())) {
            if (c == '\n' || c == '\r') {
                char d = stream.peek();
                if (d == '\n' || d == '\r') {
                    stream.get();
                    return;
                }
            }
            line.push_back(c);
        }
    }
}