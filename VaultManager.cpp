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
        vaultPath = configManager->getVaultPath().string() + "vault.xml";
        
        //If XML file does not exist: create a new file
        if (!std::filesystem::exists(configManager->getVaultPath()))
            std::filesystem::create_directory(configManager->getVaultPath());
        if (!std::filesystem::exists(vaultPath))
        {
                //Create the new XML file
            std::cout << "Could not find XML file inside Vault.\nCreating new." << std::endl;
            std::fstream vaultFile(vaultPath, std::ios_base::out);
            vaultFile.write("<vault>\n</vault>", 17);
            vaultFile.close();
        }
        document.LoadFile(vaultPath.string().c_str());
        rootNode = document.RootElement();
    }
        
        //Backup a file with path or file object
            //Returns negative if failed; else the file's ID number
    int VaultManager::fileBackup(std::filesystem::path file)
    {
        //The path after the file is put in vault
        std::filesystem::path fileVaultPath = configManager->getVaultPath() / file;
        int backupID;
        try
        {
            if (std::filesystem::exists(fileVaultPath))
            {
                std::cout << "Designated file already exists in registry.\n"
                    << "The backed up file will have 'copy' prepended." << std::endl;
                int copy = 0;
                std::filesystem::path incrementedPath;
                std::string filenameNoExt = fileVaultPath.filename().string();
                filenameNoExt.erase(filenameNoExt.length() - fileVaultPath.extension().string().length());
                do
                {
                    if (copy == 0)
                    {
                        incrementedPath = fileVaultPath;
                        incrementedPath.replace_filename(filenameNoExt + "_copy" + fileVaultPath.extension().string());
                    }
                    else
                    {
                        incrementedPath = fileVaultPath;
                        incrementedPath.replace_filename
                            (filenameNoExt + "_" + "copy" + std::to_string(copy) + fileVaultPath.extension().string());
                    }
                    copy++;
                } while (std::filesystem::exists(incrementedPath));
                fileVaultPath = incrementedPath;
            }

            std::filesystem::copy(file, fileVaultPath);
            std::cout << "File \n\t'"
                << file.filename().string()
                << "'\nhas been backed up to:\n\t"
                << configManager->getVaultPath()
                << std::endl;

            //Log this backup in XML file
            backupID = getFirstAvailableID();
            logBackup(backupID, std::filesystem::current_path() / file, file.filename().string(), fileVaultPath.filename().string());

            std::cout << "Your file has been assigned ID "
                << backupID << std::endl;
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
        tinyxml2::XMLElement *file = rootNode->FirstChildElement("file");
        while(file != NULL)
        {
            //Check this file's name against the provided filename
            if (strcmp(identifier.c_str(), file->FirstChildElement("fileName")->GetText()) == 0)
                break;
            file = file->NextSiblingElement();
        }
        if (file == NULL)
        {
            std::cout << "We could not find " << identifier
                << " in the vault. Use 'query' to search."
                << std::endl;
            return;
        }
        else
        {
            //Attempt to move the found file to where it was called from
            std::filesystem::path backupFileName = std::filesystem::current_path() / identifier;
            if (std::filesystem::exists(backupFileName))
            {
                std::cout << "File with same name already exists.\n"
                    << "File will be retrieved as '_copy'." << std::endl;
                int copy = 0;
                std::filesystem::path incrementedPath;
                std::string filenameNoExt = backupFileName.filename().string();
                filenameNoExt.erase(filenameNoExt.length() - backupFileName.extension().string().length());
                do
                {
                    if (copy == 0)
                    {
                        incrementedPath = backupFileName;
                        incrementedPath.replace_filename(filenameNoExt + "_copy" + backupFileName.extension().string());
                    }
                    else
                    {
                        incrementedPath = backupFileName;
                        incrementedPath.replace_filename
                            (filenameNoExt + "_" + "copy" + std::to_string(copy) + backupFileName.extension().string());
                    }
                    copy++;
                } while (std::filesystem::exists(incrementedPath));
                backupFileName = incrementedPath;
            }
            std::filesystem::copy(
                configManager->getVaultPath() / file->FirstChildElement("backupName")->GetText(),
                std::filesystem::current_path() / backupFileName);
        }
    }
    //void VaultManager::fileRetrieve(int identifier);
        //Return whether a file matches specified query
            //Can return all close-matching results (optional)
    std::FILE fileQuery(std::string query);
    std::FILE* fileQuery(std::string query, bool similar);
        //TODO: Add code

    void VaultManager::logBackup(int id, std::filesystem::path filePath, std::string fileName, std::string backupName)
    {
        tinyxml2::XMLElement *newBackup = document.NewElement("file");
        newBackup->InsertNewChildElement("id")->InsertNewText(std::to_string(id).c_str());
        newBackup->InsertNewChildElement("fileName")->InsertNewText(fileName.c_str());
        newBackup->InsertNewChildElement("backupName")->InsertNewText(backupName.c_str());
        newBackup->InsertNewChildElement("filePath")->InsertNewText(filePath.string().c_str());
        time_t currentTime = time(0);
        std::string currentTimeString = ctime(&currentTime);
        currentTimeString.erase(std::remove(currentTimeString.begin(), currentTimeString.end(), '\n'), currentTimeString.end()); //Remove any newlines
        currentTimeString.erase(std::remove(currentTimeString.begin(), currentTimeString.end(), '\r'), currentTimeString.end()); //Remove any newlines
        newBackup->InsertNewChildElement("backupDate")->InsertNewText(currentTimeString.c_str());
        rootNode->LinkEndChild(newBackup);
        document.SaveFile(vaultPath.string().c_str());
    }
    int VaultManager::getFirstAvailableID()
    {
        int id = 0;
        tinyxml2::XMLElement *element = rootNode->FirstChildElement("file");
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