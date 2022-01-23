#ifndef __VAULTMANAGER_H__
#define __VAULTMANAGER_H__

#include <fstream>
#include <filesystem>
#include "backfile.h"
#include "configmanager.h"
//A class which will handle a vault folder
	//Making sure the vault has all required files
	//Retrievs files and manages the files in the vault
class VaultManager
{
    public:
        VaultManager(ConfigManager *manager);
        int fileBackup(const std::filesystem::path& file);
            //Get a file with string- or integer identifier
        void fileRetrieve(const std::string& identifier);
        void fileRetrieve(int identifier);
            //Return whether a file matches specified query
                //Can return all close-matching results (optional)
        BackFile* fileQuery(const std::string& query, bool verbose = true);

		std::vector<BackFile*> getFiles();

    private:
        ConfigManager *configManager;
        std::filesystem::path vaultPath;
        tinyxml2::XMLDocument document;
        tinyxml2::XMLElement *rootNode;
        void logBackup(int id, const std::filesystem::path& filePath, const std::string& fileName, const std::string& backupName);
        int getFirstAvailableID();
};
#endif // __VAULTMANAGER_H__