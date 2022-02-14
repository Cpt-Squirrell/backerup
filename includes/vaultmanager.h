#ifndef __VAULTMANAGER_H__
#define __VAULTMANAGER_H__

#include <vector>
#include <fstream>
#include <filesystem>
#include "backfile.h"
#include "configmanager.h"

//A class which will handle a vault folder
//Making sure the vault has all required files
//Retrieves files and manages the files in the vault
class VaultManager {
public:
	explicit VaultManager(ConfigManager *manager);
	~VaultManager();

	//Put &file into vault, taken from the application pwd
	int fileBackup(const std::filesystem::path &file);
	//Place &identifier where the application was called from
	void fileRetrieve(const std::string &identifier);
	//Place a file back to where it was originally backed from
	void fileRestore(const std::string &identifier, bool replace = false);
	//Will return the closest matching file and with verbose prints it + duplicates + similar
	BackFile *fileQuery(const std::string &query, bool verbose = true);
	//Print all files in the vault
	void listFiles();

private:
	//Variables
	ConfigManager *configManager;
	std::filesystem::path vaultPath;
	tinyxml2::XMLDocument document;
	tinyxml2::XMLElement *rootNode;
	std::vector<BackFile*> files;

	//Functions
	std::vector<BackFile *> getFiles(); //Return an up-to-date vector of all files in vault
	BackFile* parseFile(const std::string &identifier); //Return the file which exactly equals &identifier
	BackFile* parseFile(int identifier); //Return the file which exactly equals &identifier
	static std::filesystem::path createUniquePath //Appends incremental "copy"-ies until file path is valid
		(std::filesystem::path destination, const std::filesystem::path& source);
	void logBackup //Create an entry in vault XML
		(int id, const std::filesystem::path &filePath, const std::string &fileName, const std::string &backupName);
	int firstAvailableID(); //Return the first ID which is unused in vault XML
};

#endif // __VAULTMANAGER_H__