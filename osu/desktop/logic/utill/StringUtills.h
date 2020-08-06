//
// Created by MasterLogick on 4/21/20.
//

#ifndef OSU_LASER_C_STRINGUTILLS_H
#define OSU_LASER_C_STRINGUTILLS_H

#include <string>
#include <vector>

namespace osu {
    std::pair<std::string, std::string> splitKeyValPair(std::string &kpVar, char separator = ':');

    std::pair<std::string, std::string> splitKeyValPair(std::string &kpVar, int skip, char separator = ':');

    std::vector<std::string> split(std::string &str, std::string separators);

    std::vector<std::string> split(std::string &str, char separator);

    bool startsWith(std::string &s, const char *prefix);
}
#endif //OSU_LASER_C_STRINGUTILLS_H
