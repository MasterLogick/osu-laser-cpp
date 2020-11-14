//
// Created by MasterLogick on 7/17/20.
//

#include "BufferedReader.h"
#include "StringUtills.h"
#include "stringbuilder.h"
#include <cstring>
#include <iostream>

namespace osu {


    BufferedReader::BufferedReader(std::istream &inputStream) : stream(inputStream), markedChar(-1), readAheadLimit(0),
                                                                skipLF(false), markedSkipLF(false), nextChar(0),
                                                                nChars(0) {
        cb = new char[bufferSize];
    }

    bool BufferedReader::readLine(std::string &line) {
        std::string *s = nullptr;
        int startChar;

        for (;;) {

            if (nextChar >= nChars)
                fill();
            if (nextChar >= nChars) {
                if (s != nullptr && s->length() > 0) {
                    line = *s;
                    delete s;
                    return true;
                } else {
                    delete s;
                    return false;
                }
            }
            bool eol = false;
            char c = 0;
            int i;

            skipLF = false;

            for (i = nextChar; i < nChars; i++) {
                c = cb[i];
                if ((c == '\n') || (c == '\r')) {
                    eol = true;
                    break;
                }
            }

            startChar = nextChar;
            nextChar = i;

            if (eol) {
                std::string str;
                if (s == nullptr) {
                    str.append(cb + startChar, i - startChar);
                } else {
                    s->append(cb + startChar, i - startChar);
                    str = *s;
                }
                nextChar++;
                if (c == '\r') {
                    skipLF = true;
                }
                line = str;
                delete s;
                return true;
            }

            if (s == nullptr)
                s = new std::string();
            s->append(cb + startChar, i - startChar);
        }

    }

    void BufferedReader::fill() {
        int dst;
        if (markedChar <= UNMARKED) {
            /* No mark */
            dst = 0;
        } else {
            /* Marked */
            int delta = nextChar - markedChar;
            if (delta >= readAheadLimit) {
                /* Gone past read-ahead limit: Invalidate mark */
                markedChar = INVALIDATED;
                readAheadLimit = 0;
                dst = 0;
            } else {
                if (readAheadLimit <= bufferSize) {
                    std::memcpy(cb, cb + markedChar, delta);
                    markedChar = 0;
                    dst = delta;
                } else {
                    /* Reallocate buffer to accommodate read-ahead limit */
                    char *ncb = new char[readAheadLimit];
                    std::memcpy(ncb, cb + markedChar, delta);
                    delete[] cb;
                    cb = ncb;
                    bufferSize = readAheadLimit;
                    markedChar = 0;
                    dst = delta;
                }
                nextChar = nChars = delta;
            }
        }
        int n;
        do {
            try {
                stream.read(cb + dst, bufferSize - dst);
            } catch (...) {}
            n = stream.gcount();
        } while (n == 0 && stream.good());
        if (n > 0) {
            nChars = dst + n;
            nextChar = dst;
        }
    }

    BufferedReader::~BufferedReader() {
        delete[] cb;
    }
}