#include <iostream>
#include <windows.h>
#include <filesystem>
#include "includes\tinyxml2.h"
#include "includes\configmanager.h"

    ConfigManager::ConfigManager()
    {
        //TODO: Create new Config XML if existing is not found!
        document = new tinyxml2::XMLDocument();
        try
        {
            document->LoadFile("config.xml");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return;
        }
        workingDirectoryPath = std::filesystem::current_path();
        rootNode = document->RootElement();
        nodeVaultPath = rootNode->FirstChildElement("vaultPath");
    }
    ConfigManager::~ConfigManager()
    {
        document->SaveFile("Configuration.xml");
        delete document;
    }
    
    void ConfigManager::setConfig(configOptions option, std::string value)
    {
    }
    std::string ConfigManager::getConfig(configOptions option)
    {
        //TODO: Finish
        switch (option)
        {
            case vaultPath: return nodeVaultPath->GetText();
            default: return 0;
        }
    }
    std::string ConfigManager::workingDirectory()
    {
        return workingDirectoryPath.string();
    }
    
    std::filesystem::path workingDirectoryPath;
    std::filesystem::path configFilePath = std::filesystem::path("configuration.xml");
    tinyxml2::XMLDocument *document;
    tinyxml2::XMLElement *rootNode;
    tinyxml2::XMLElement *nodeVaultPath;