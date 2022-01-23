#ifndef BACKERUP_BACKFILE_H
#define BACKERUP_BACKFILE_H

#include <string>
#include "configmanager.h"
class BackFile
{
public:
	BackFile(ConfigManager *configManager = nullptr, int idFile = 0);
	std::string getName();
	std::string getBackupName();
	std::filesystem::path getPath();
	std::filesystem::path getLocation();
	std::time_t *getDate();

private:
	std::string name, backupName;
	std::filesystem::path filePath, location;
	std::time_t *date;
};

#endif //BACKERUP_BACKFILE_H
