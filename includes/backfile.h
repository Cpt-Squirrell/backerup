#ifndef BACKERUP_BACKFILE_H
#define BACKERUP_BACKFILE_H

#include <string>
#include "configmanager.h"
class BackFile
{
public:
	explicit BackFile(ConfigManager *configManager = nullptr, int idFile = 0);
	~BackFile();
	[[nodiscard]] int getID() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getBackupName() const;
    [[nodiscard]] std::filesystem::path getPath() const;
    [[nodiscard]] std::time_t* getDate() const;

private:
	int id;
	std::string name, backupName;
	std::filesystem::path filePath;
	std::time_t *date;
};

#endif //BACKERUP_BACKFILE_H
