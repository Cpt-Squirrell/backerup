#include <fstream>
#include <iostream>
#include <windows.h>
#include <filesystem>
#include "includes\tinyxml2.h"
#include "includes\configmanager.h"

    ConfigManager::ConfigManager(const char *applicationPath)
    {
        configPath = applicationPath;
        configPath.remove_filename();
        configPath += "config.xml";

            //Check if the config file exists
                //If not: Create new
        if (!std::filesystem::exists(configPath))
        {
            std::fstream configFile(configPath, std::ios_base::out);
            configFile.write("<config>\n</config>", 19);
            configFile.close();
        }

            //Load the XML document for the parser
            //And assign the root element
        document.LoadFile(configPath.string().c_str());
        rootNode = document.RootElement();
        
        if (rootNode->FirstChildElement("vaultPath") == NULL)
        {
            vaultPath = applicationPath;
            vaultPath.remove_filename();
            vaultPath += "vault\\";
            tinyxml2::XMLElement *vaultElement = document.NewElement("vaultPath");
            vaultElement->SetText(vaultPath.string().c_str());
            rootNode->InsertFirstChild(vaultElement);
            document.SaveFile(configPath.string().c_str());
        }
        else
        {
            vaultPath = rootNode->FirstChildElement("vaultPath")->GetText();
        }
    }
    ConfigManager::~ConfigManager()
    {
        document.SaveFile(configPath.string().c_str());
    }

    std::filesystem::path ConfigManager::getVaultPath()
    {
        return vaultPath;
    }