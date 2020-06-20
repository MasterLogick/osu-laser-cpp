//
// Created by MasterLogick on 20/06/2020.
//

#include <string>
#include <iostream>

int main(int argc, const char **argv, const char **envp) {
    std::string s{"aaa"};
    std::string a = s.substr(1);
    s.data()[1] = 'b';
    std::cout << a << std::endl;
}