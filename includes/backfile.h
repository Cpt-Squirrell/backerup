#ifndef BACKERUP_BACKFILE_H
#define BACKERUP_BACKFILE_H

#include <filesystem>
#include "tinyxml2.h"
#include "configmanager.h"

class backFile
{
public:
	backFile(ConfigManager *confManager, int idFile);
	std::string getName();
	std::string getBackupName();
	std::filesystem::path getPath();
	std::time_t getDate();

private:
	std::string name;
	std::string backupName;
	std::filesystem::path filePath;
	std::time_t date;
};

#endif //BACKERUP_BACKFILE_H
