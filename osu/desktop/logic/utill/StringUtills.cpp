//
// Created by MasterLogick on 7/3/20.
//

#include "StringUtills.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>

namespace osu {

    std::pair<std::string, std::string> splitKeyValPair(std::string &kpVar, char separator) {
        size_t pos = kpVar.find(separator);
        std::string val{kpVar.substr(pos + 1)};
        return std::pair<std::string, std::string>(boost::trim_copy(kpVar.substr(0, pos)), boost::trim_copy(val));
    }

    std::pair<std::string, std::string> splitKeyValPair(std::string &kpVar, int skip, char separator) {
        size_t pos = -1;
        for (int i = 0; i < skip; ++i) {
            pos = kpVar.find(separator, pos + 1);
        }
        pos = kpVar.find(separator, pos + 1);
        std::string val{kpVar.substr(pos + 1)};
        return std::pair<std::string, std::string>(kpVar.substr(0, pos), val);
    }

    std::vector<std::string> split(std::string &str, std::string separators) {
        std::vector<std::string> result;
        boost::split(result, str, boost::is_any_of(separators));
        return result;
    }

    std::vector<std::string> split(std::string &str, char separator) {
        std::vector<std::string> result;
        boost::split(result, str, [separator](char c) { return c == separator; });
        return result;
    }

    bool startsWith(std::string &s, const char *prefix) {
        const char *ptr = s.c_str();
        size_t size = s.size();
        for (int i = 0; prefix[i] != '\0'; i++) {
            if (size == i || ptr[i] != prefix[i]) { return false; }
        }
        return true;
    }
}