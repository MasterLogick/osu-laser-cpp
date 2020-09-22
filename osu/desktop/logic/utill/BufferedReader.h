//
// Created by MasterLogick on 7/17/20.
//

#ifndef OSU_LASER_CPP_BUFFEREDREADER_H
#define OSU_LASER_CPP_BUFFEREDREADER_H

#include <istream>
#include <list>

namespace osu {
    class BufferedReader {
#define INVALIDATED -2
#define UNMARKED -1
        std::istream &stream;
        std::string buffer;
        size_t bufferSize{8192};
        char *cb;
        int nChars, nextChar;
        int markedChar = UNMARKED;
        int readAheadLimit = 0;
        bool skipLF = false;
        bool markedSkipLF = false;

        void fill();

    public:

        explicit BufferedReader(std::istream &inputStream);

        bool readLine(std::string &line);
    };
}

#endif //OSU_LASER_CPP_BUFFEREDREADER_H
