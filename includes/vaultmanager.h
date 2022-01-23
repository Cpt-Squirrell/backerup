#ifndef __VAULTMANAGER_H__
#define __VAULTMANAGER_H__

#include <vector>
#include <fstream>
#include <filesystem>
#include "backfile.h"
#include "configmanager.h"

//A class which will handle a vault folder
//Making sure the vault has all required files
//Retrievs files and manages the files in the vault
class VaultManager {
public:
	explicit VaultManager(ConfigManager *manager);

	int fileBackup(const std::filesystem::path &file);

	//Get a file with string- or integer identifier
	void fileRetrieve(const std::string &identifier);
	void fileRestore(const std::string &identifier, bool replace = false);
	//Return whether a file matches specified query
	//Can return all close-matching results (optional)
	BackFile *fileQuery(const std::string &query, bool verbose = true);

	void listFiles();

	std::vector<BackFile *> getFiles();

	std::vector<BackFile *> vaultList();

private:
	ConfigManager *configManager;
	std::filesystem::path vaultPath;
	tinyxml2::XMLDocument document;
	tinyxml2::XMLElement *rootNode;

	BackFile* parseFile(const std::string &identifier);
	std::filesystem::path createUniquePath(std::filesystem::path destination, const std::filesystem::path& source);

	void logBackup
		(int id, const std::filesystem::path &filePath, const std::string &fileName, const std::string &backupName);

	int firstAvailableID();
};

#endif // __VAULTMANAGER_H__