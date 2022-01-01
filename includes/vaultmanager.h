#ifndef __VAULTMANAGER_H__
#define __VAULTMANAGER_H__

#include <fstream>
#include <filesystem>
#include "includes\configmanager.h"

    //A class which will handle a vault folder
        //Making sure the vault has all required files
        //Retrievs files and manages the files in the vault
class VaultManager
{
    public:
        VaultManager(ConfigManager *manager);
        int fileBackup(std::filesystem::path file);
            //Get a file with string- or integer identifier
        void fileRetrieve(std::string identifier);
        void fileRetrieve(int identifier);
            //Return whether a file matches specified query
                //Can return all close-matching results (optional)
        std::FILE fileQuery(std::string query);
        std::FILE* fileQuery(std::string query, bool similar);

    private:
        ConfigManager *configManager;
        std::filesystem::path vaultPath;
        tinyxml2::XMLDocument *vaultXML;
        tinyxml2::XMLElement *rootXML;
        void logBackup(int id, std::filesystem::path filePath, std::string fileName);
        int getFirstAvailableID();
};
#endif // __VAULTMANAGER_H__