//
// Created by MasterLogick on 4/21/20.
//

#ifndef OSU_LASER_C_STRINGUTILLS_H
#define OSU_LASER_C_STRINGUTILLS_H

#include <string>
#include <vector>

namespace osu {
    void ltrim(std::string &s);

    void rtrim(std::string &s);

    void trim(std::string &s);

    std::string ltrim_copy(std::string &s);

    std::string rtrim_copy(std::string &s);

    std::string trim_copy(std::string &s);

    std::pair<std::string, std::string> splitKeyValPair(std::string &kpVar, char separator = ':');

    std::pair<std::string, std::string> splitKeyValPair(std::string &kpVar, int skip, char separator = ':');

    std::vector<std::string> split(std::string &str, std::string separator);

    bool startsWith(std::string &s, const char *prefix);
}
#endif //OSU_LASER_C_STRINGUTILLS_H
