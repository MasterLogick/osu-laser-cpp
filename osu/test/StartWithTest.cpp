//
// Created by MasterLogick on 28/06/2020.
//

#include <string>
#include <cstdlib>
#include <logic/utill/StringUtills.h>

using namespace osu;

int main(int argc, const char **argv, const char **envp) {
    std::string s{"abcde"};
    if (!startsWith(s, "a")) { return 1; }
    if (!startsWith(s, "ab")) { return 1; }
    if (!startsWith(s, "abc")) { return 1; }
    if (!startsWith(s, "abcd")) { return 1; }
    if (!startsWith(s, "abcde")) { return 1; }
    if (startsWith(s, "abcdef")) { return 1; }
    if (startsWith(s, "abbdef")) { return 1; }
    if (startsWith(s, "abde")) { return 1; }
    return 0;
}