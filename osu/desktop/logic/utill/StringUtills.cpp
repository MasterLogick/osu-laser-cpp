//
// Created by MasterLogick on 7/3/20.
//

#include "StringUtills.h"
#include <algorithm>
#include <cctype>
#include <locale>
#include <boost/algorithm/string.hpp>

namespace osu {
    void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    std::string ltrim_copy(std::string &s) {
        ltrim(s);
        return s;
    }

    std::string rtrim_copy(std::string &s) {
        rtrim(s);
        return s;
    }

    std::string trim_copy(std::string &s) {
        trim(s);
        return s;
    }

    std::pair<std::string, std::string> splitKeyValPair(std::string &kpVar, char separator) {
        size_t pos = kpVar.find(separator);
        std::string val{kpVar.substr(pos + 1)};
        ltrim(val);
        return std::pair<std::string, std::string>(kpVar.substr(0, pos), val);
    }

    std::pair<std::string, std::string> splitKeyValPair(std::string &kpVar, int skip, char separator) {
        size_t pos = -1;
        for (int i = 0; i < skip; ++i) {
            pos = kpVar.find(separator, pos + 1);
        }
        pos = kpVar.find(separator, pos + 1);
        std::string val{kpVar.substr(pos + 1)};
        ltrim(val);
        return std::pair<std::string, std::string>(kpVar.substr(0, pos), val);
    }

    std::vector<std::string> split(std::string &str, std::string separator) {
        std::vector<std::string> result;
        boost::split(result, str, boost::is_any_of(separator));
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