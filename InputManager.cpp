#include <string>
#include <iostream>
#include <filesystem>
#include "includes\inputmanager.h"

    InputManager::InputManager(int argc, char *arguments[])
    {
        //Find the option
        std::string optionString = arguments[1];
        if (!optionString.compare("backup"))
            option = Options::backup;
        else if (!optionString.compare("retrieve"))
            option = Options::retrieve;
        else if (!optionString.compare("setConfig"))
            option = Options::setConfig;
        else if (!optionString.compare("getConfig"))
            option = Options::getConfig;

        if (option == Options::backup || option == Options::retrieve)
            argument = arguments[argc - 1]; //TODO: Might be argc (not - 1)
    }

    InputManager::Options InputManager::getOption()
    {
        return option;
    }
    InputManager::Flags* InputManager::getFlags()
    {
        return flags;
    }
    std::string InputManager::getArgument()
    {
        return argument;
    }