//
// Created by user on 1/19/20.
//
#ifdef __linux__
#define CONFIG_PATH "$HOME/.config/osu-laser-c++/config.cfg"
#endif
#ifdef __MINGW32__
#define CONFIG_PATH "%APPDATA%/osu-laser-c++/config.cfg"
#endif
#ifdef __APPLE__
#define CONFIG_PATH "~/Library/Application Support/osu-laser-c++"
#endif