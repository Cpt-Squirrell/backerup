#ifndef __CONFIGMANAGER_H__
#define __CONFIGMANAGER_H__

#include <string>
#include <filesystem>
#include "includes\tinyxml2.h"
    //A class for reading and writing to the configuration file
class ConfigManager
{
    public:
        enum configOptions
        {
            vaultPath
        };
        ConfigManager();
        ~ConfigManager();
        void setConfig(configOptions option, std::string value);
        std::string getConfig(configOptions option);
        std::string workingDirectory();

    private:
        std::filesystem::path workingDirectoryPath;
        std::filesystem::path configFilePath = std::filesystem::path("configuration.xml");
        tinyxml2::XMLDocument *document;
        tinyxml2::XMLElement *rootNode;
        tinyxml2::XMLElement *nodeVaultPath;
};
#endif // __CONFIGMANAGER_H__