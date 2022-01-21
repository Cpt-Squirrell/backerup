#ifndef __CONFIGMANAGER_H__
#define __CONFIGMANAGER_H__

#include <string>
#include <filesystem>
#include "tinyxml2.h"
//A class for reading and writing to the configuration file
class ConfigManager
{
    public:
        explicit ConfigManager(const char *applicationPath);
        ~ConfigManager();
        std::filesystem::path getVaultPath();

    private:
        std::filesystem::path vaultPath;
        std::filesystem::path configPath;
        tinyxml2::XMLDocument document;
        tinyxml2::XMLElement *rootNode;
};
#endif // __CONFIGMANAGER_H__