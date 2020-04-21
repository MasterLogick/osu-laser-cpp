//
// Created by MasterLogick on 1/19/20.
//

#include "Properties.h"
#include "platformDependentDefines.h"
#include "../graphics/GLFWWindow.h"

namespace osu {

    Config Properties::cfg{};
    Setting *Properties::VideoMode = nullptr;
    Setting *Properties::Title = nullptr;
    Setting *Properties::MSAALevel = nullptr;
    Setting *Properties::KeyBindings = nullptr;

    void Properties::initialise() {
        try {
            cfg.readFile(CONFIG_PATH);
        } catch (const FileIOException &fioex) {
            /*std::cerr << "I/O error while reading file." << std::endl; //todo handle exception
            return(EXIT_FAILURE);*/
        } catch (const ParseException &pex) {
            /* std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine() //todo handle exception
                       << " - " << pex.getError() << std::endl;
             return(EXIT_FAILURE);*/
        }
        cfg.setOptions(Config::OptionFsync | Config::OptionSemicolonSeparators |
                       Config::OptionColonAssignmentForGroups | Config::OptionOpenBraceOnSeparateLine);
        Setting &root = cfg.getRoot();
        if (root.exists("VideoMode")) {
            Properties::VideoMode = &root.lookup("VideoMode");
        } else {
            Properties::VideoMode = &root.add("VideoMode", Setting::TypeGroup);
        }
        if (!Properties::VideoMode->exists("Width")) {
            Properties::VideoMode->add("Width", Setting::TypeInt) = 1366;
        }
        if (!Properties::VideoMode->exists("Height")) {
            Properties::VideoMode->add("Height", Setting::TypeInt) = 786;
        }
        if (!Properties::VideoMode->exists("RefreshRate")) {
            Properties::VideoMode->add("RefreshRate", Setting::TypeInt) = 60;
        }
        if (!Properties::VideoMode->exists("Type")) {
            Properties::VideoMode->add("Type", libconfig::Setting::TypeInt) = TYPE_WINDOW;
        }
        if (root.exists("KeyBindings")) {
            Properties::KeyBindings = &root.lookup("KeyBindings");
        } else {
            Properties::KeyBindings = &root.add("KeyBindings", Setting::TypeString);
            //todo add default key bindings
        }
        if (root.exists("MSAALevel"))
            Properties::MSAALevel = &root.lookup("MSAALevel");
        else {
            Properties::MSAALevel = &root.add("MSAALevel", Setting::TypeInt);
            *Properties::MSAALevel = 4;
        }
        if (root.exists("Title"))
            Properties::Title = &root.lookup("Title");
        else {
            Properties::Title = &root.add("Title", Setting::TypeString);
            *Properties::Title = "osu!laser#c++";
        }
        cfg.writeFile(CONFIG_PATH);
    }
}