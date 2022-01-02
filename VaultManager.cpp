#include <ctime>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "includes\tinyxml2.h"
#include "includes\configmanager.h"
#include "includes\vaultmanager.h"

    VaultManager::VaultManager(ConfigManager *manager)
    {
        configManager = manager;
        vaultPath = configManager->getConfig(ConfigManager::configOptions::vaultPath);
        vaultXML = new tinyxml2::XMLDocument();
        //If XML file does not exist: create a new file
        if (!std::filesystem::exists(vaultPath.string() + "vault.xml"))
        {
            std::cout << "Could not find XML file inside Vault.\nCreating new." << std::endl;
            std::fstream *vaultFile = new std::fstream();
            vaultFile->open(vaultPath.string() + "vault.xml");
            vaultFile->close();
        }
        vaultXML->LoadFile((vaultPath.string() + "vault.xml").c_str()); //TODO: Might not work
        rootXML = vaultXML->RootElement();
    }
        //Backup a file with path or file object
            //Returns negative if failed; else the file's ID number
    int VaultManager::fileBackup(std::filesystem::path file)
    {
        std::filesystem::path qualifiedPath =
            configManager->getConfig(ConfigManager::configOptions::vaultPath) +
                file.filename().string();
        int backupID;
        std::error_code *copyError = new std::error_code();
        try
        {
            while(std::filesystem::exists(qualifiedPath))
            {
                std::cout << "Designated file already exists in registry.\n" <<
                    " File with be backed up designated as 'copy'." << std::endl;
                qualifiedPath = qualifiedPath.string() + ".copy" + qualifiedPath.extension().string();
                //TODO: Does this work? lol Update: Yes it does kekw
                //TODO: Remove these clutter comments omegalol
            }
            std::filesystem::copy(file, qualifiedPath);
            std::cout << "File '"
                << file.filename().string()
                << "' has been backed up to:\n"
                << configManager->getConfig(ConfigManager::configOptions::vaultPath)
                << std::endl;

            //Log this backup in XML file
            backupID = getFirstAvailableID();
            logBackup(backupID, file, file.filename().string());
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return -1;
        }

        return backupID;
    }
        //Get a file with string- or integer identifier
    void VaultManager::fileRetrieve(std::string identifier)
    {
    }
    void VaultManager::fileRetrieve(int identifier)
    {
    }
        //Return whether a file matches specified query
            //Can return all close-matching results (optional)
    std::FILE fileQuery(std::string query);
    std::FILE* fileQuery(std::string query, bool similar);
        //TODO: Add code

    ConfigManager *configManager;
    std::filesystem::path vaultPath;
    tinyxml2::XMLDocument *vaultXML;
    tinyxml2::XMLElement *rootXML;

    void VaultManager::logBackup(int id, std::filesystem::path filePath, std::string fileName)
    {
        tinyxml2::XMLElement *newBackup = vaultXML->NewElement("file");
        newBackup->InsertNewChildElement("id")->InsertNewText(std::to_string(id).c_str());
        newBackup->InsertNewChildElement("fileName")->InsertNewText(fileName.c_str());
        newBackup->InsertNewChildElement("filePath")->InsertNewText(filePath.string().c_str());
        time_t currentTime = time(0);
        newBackup->InsertNewChildElement("fileBackupDate")->InsertNewText(ctime(&currentTime));
        rootXML->LinkEndChild(newBackup);
        vaultXML->SaveFile((vaultPath.string() + "vault.xml").c_str());
    }
    int VaultManager::getFirstAvailableID()
    {
        int id = 0;
        tinyxml2::XMLElement *element = rootXML->FirstChildElement("file");
        if (element == NULL)
            return id;
        bool available = false;
        while(!available)
        {
            available = true;
            while(element != NULL)
            {
                if(element->FirstChildElement("id")->GetText() == std::to_string(id))
                {
                    available = false;
                    break;
                }
                element = element->NextSiblingElement();
            }
            if (!available)
                id++;
        }

        return id;
    }