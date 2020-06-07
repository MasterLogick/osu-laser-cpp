//
// Created by MasterLogick on 6/5/20.
//

#include "../BeatmapLoader.h"
#include "../../utill/StringUtills.h"

namespace osu {
    void BeatmapLoader::loadLegacyStoryBoardFromFile(std::istream &stream) {
        std::string line;
        while (!stream.eof()) {
            getline(stream, line);
            trim(line);
            std::size_t pos = line.find("//");
            if (line.size() <= 1 || pos == 0) {
                continue;
            }
            if (line[0] == '[' && line[line.size() - 1] == ']') {
                handleSection(line);
            } else {
                switch (currientToken) {
                    case Events:
                        handleEvents(line);
                        break;
                    case Variables:
                        handleVariables(line);
                        break;
                }
            }
        }
    }

    void BeatmapLoader::handleVariables(std::string &line) {
        variables.push_back(splitKeyValPair(line, '='));
    }

    void BeatmapLoader::decodeVariables(std::string *line) {
        while (line->find('$') != std::string::npos) {
            char copys[line->length()];
            line->copy(copys, line->length());
            std::for_each(variables.begin(), variables.end(), [line](std::pair<std::string, std::string> &i) {
                boost::replace_all(*line, i.first.c_str(), i.second.c_str());
            });
            if (!line->compare(copys))return;
        }
    }

    void BeatmapLoader::handleEvents(std::string &line) {
        decodeVariables(&line);
    }
}