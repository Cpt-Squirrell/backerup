#include <ctime>
#include <vector>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include "includes/tinyxml2.h"
#include "includes/backfile.h"
#include "includes/configmanager.h"
#include "includes/vaultmanager.h"

VaultManager::VaultManager(ConfigManager *manager)
{
	configManager = manager;
	vaultPath = configManager->getVaultPath() / "vault.xml";

	//Do some checks to make sure things exists
	if (!std::filesystem::exists(configManager->getVaultPath())) //Does the vault folder exist?
	{
		//Create the folder
		std::cout << "Note: Did not find configured Vault folder. Creating new." << std::endl;
		std::filesystem::create_directory(configManager->getVaultPath());
	}
	if (!std::filesystem::exists(vaultPath)) //Does the XML exist?
	{
		//Create a new XML file
		std::cout << "Note: Could not find XML file inside Vault. Creating new." << std::endl;
		std::fstream vaultFile(vaultPath, std::ios_base::out);
		vaultFile.write("<vault>\n</vault>", 17); //Create the root node manually
		vaultFile.close();
	}

	document.LoadFile(vaultPath.string().c_str());
	rootNode = document.RootElement();
	files = getFiles();
}
VaultManager::~VaultManager() {
	document.SaveFile(vaultPath.string().c_str());
	for (BackFile* file : files) {
		delete file;
		//TODO: Does this work (altering vector inside loop)
        //Seems to work
	}
}

//Backup a file with path or file object
//Returns negative if failed; else the file's ID number
int VaultManager::fileBackup(const std::filesystem::path &file) {
	//The path after the file is put in vault
	std::filesystem::path fileVaultPath = createUniquePath(vaultPath, file);
	std::filesystem::copy(file, fileVaultPath);

	//Add a new entry in the vault XML file
	int backupID = firstAvailableID();
	logBackup(backupID, std::filesystem::current_path() / file,
	          file.filename().string(), fileVaultPath.filename().string());

	//Print the result
	if (file.filename().string().length() > 50) {
		//Filename too long, newline
		std::cout << file.filename().string() << "\n ";
	} else {
		//Filename not too long, keep same line
		std::cout << file.filename().string() << ' ';
	}
	std::cout << "has been backed up to:\n"
			  << configManager->getVaultPath()
			  << std::endl;

	std::cout << "Your file has been assigned ID " << backupID << std::endl;
	return backupID;
}

//Get a file with string- or integer identifier
void VaultManager::fileRetrieve(const std::string &identifier) {
	tinyxml2::XMLElement *file = rootNode->FirstChildElement("file");
	while (file != nullptr) {
		//Check this file's name against the provided filename
		if (strcmp(identifier.c_str(), file->FirstChildElement("fileName")->GetText()) == 0)
			break;
		file = file->NextSiblingElement();
	}
	if (file == nullptr) {
		std::cout << "We could not find " << identifier
				  << " in the vault.\n"
                  << "Use 'query' to search."
				  << std::endl;
		return;
	} else {
		//Attempt to move the found file to where it was called from
		std::filesystem::path backupFileName = std::filesystem::current_path() / identifier;
		if (std::filesystem::exists(backupFileName)) {
			std::cout << "File with same name already exists.\n"
					  << "File will be retrieved as '_copy'." << std::endl;
			int copy = 0;
			std::filesystem::path incrementedPath;
			std::string filenameNoExt = backupFileName.filename().string();
			filenameNoExt.erase(filenameNoExt.length() - backupFileName.extension().string().length());
			do {
				if (copy == 0) {
					incrementedPath = backupFileName;
					incrementedPath.replace_filename
							(filenameNoExt + "_copy" + backupFileName.extension().string());
				} else {
					incrementedPath = backupFileName;
					incrementedPath.replace_filename
							(filenameNoExt + "_" + "copy" + std::to_string(copy)
							 + backupFileName.extension().string());
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
void VaultManager::fileRestore(const std::string &identifier, bool replace)
{
	BackFile *file = parseFile(identifier);
	if (file == nullptr)
		std::cout << "Could not find " << identifier << '.' << std::endl;
	if (std::filesystem::exists(file->getPath()) && !replace) {
		std::cout << "Existing file at destination exists." << std::endl;
		std::filesystem::copy
			(vaultPath / identifier, createUniquePath(file->getPath().parent_path(), file->getPath()));
	}
}

//Return whether a file matches specified query
//Can return all close-matching results (optional)
BackFile* VaultManager::fileQuery(const std::string &query, bool verbose) {
    //Make sure there are files logged
    if (getFiles().empty()) {
        if (verbose) {
            std::cout << "There are no backed up files logged in XML." << std::endl;
            return nullptr;
        } else
            return nullptr;
    }

	std::vector<BackFile *> result, duplicates;
	int bestMatchInt = 0;
	for (BackFile *file: files) {
		std::string name = file->getName();
		int matchingChars = 0;
		for (int i = 0; i < file->getName().size(); i++) {
			if (name.c_str()[i] == query.c_str()[i])
				matchingChars++;
		}
		if (matchingChars > bestMatchInt) {
			bestMatchInt = matchingChars;
			result.push_back(file);
		}
	}

	if (verbose) //Only output text if it's what the user wants
	{
		BackFile *matchingFile = result[result.size() - 1];
		std::cout << "Best match found: " << matchingFile->getName() << ",\n"
				  << "backed up as: " << matchingFile->getBackupName() << '.' << std::endl;
		for (BackFile *file: files) {
			if (file->getName() == matchingFile->getName())
				duplicates.push_back(file);
		}

		if (!duplicates.empty()) //TODO: Does not list all duplicates
		{
			std::cout << "Duplicates were found: \n";
			for (BackFile *file: duplicates) {
				std::cout << file->getBackupName() << "\n\t"
						  << std::ctime((const time_t *) file->getDate());
			}
			std::cout << std::endl;
		}

		if (files.size() > 1) //TODO: Untested feature
		{
			std::cout << "Similarly found files: \n";
			for (int i = files.size() - 1; i > files.size() - 6; i--) {
				std::cout << files[i]->getName() << '\n';
			}
			std::cout << std::endl;
		}
	}

	return result[result.size() - 1];
}

void VaultManager::listFiles() {
	for (BackFile *file: files) {
		if (file->getName() == file->getBackupName())
			std::cout << file->getName() << "\n\t"
					  << ctime((const time_t *) file->getDate()) << std::endl;
		else
			std::cout << file->getName() << "\n\t"
					  << file->getBackupName() << "\n\t"
					  << ctime((const time_t *) file->getDate()) << std::endl;
	}
}

//Returns a vector of all files in XML
std::vector<BackFile *> VaultManager::getFiles() {
	if (!document.RootElement()->FirstChildElement("file"))
		return {};
	tinyxml2::XMLElement *element = document.RootElement()->FirstChildElement("file");
	std::vector<BackFile *> returnVector;
	do {
		returnVector.push_back(new BackFile(configManager, std::stoi(element->FirstChildElement("id")->GetText())));
		element = element->NextSiblingElement();
	} while (element != nullptr);
	return returnVector;
}

BackFile* VaultManager::parseFile(const std::string &identifier) {
	for (BackFile *file : files)
	{
		if (identifier == file->getName())
			return file;
	}
	return nullptr;
}
BackFile* VaultManager::parseFile(const int identifier) {
	for (BackFile *file : files)
	{
		if (identifier == file->getID())
			return file;
	}
	return nullptr;
}
std::filesystem::path VaultManager::createUniquePath(std::filesystem::path destination, const std::filesystem::path& source)
{
	if (!source.has_filename())
		return {};
	if (!is_directory(destination))
		destination = destination.parent_path();
	std::filesystem::path sourceFile = source.filename();
	int increment = 0;
	while (std::filesystem::exists(destination / sourceFile))
	{
		if (increment == 0)
			sourceFile = source.filename().string() + "_copy";
		else
			sourceFile = source.filename().string() + ("_copy" + std::to_string(increment));
		increment++;
	}
	return destination / sourceFile;
}

void VaultManager::logBackup
		(const int id, const std::filesystem::path &filePath, const std::string &fileName,
		 const std::string &backupName) {
	tinyxml2::XMLElement *newBackup = document.NewElement("file");
	newBackup->InsertNewChildElement("id")->InsertNewText(std::to_string(id).c_str());
	newBackup->InsertNewChildElement("fileName")->InsertNewText(fileName.c_str());
	newBackup->InsertNewChildElement("backupName")->InsertNewText(backupName.c_str());
	newBackup->InsertNewChildElement("filePath")->InsertNewText(filePath.string().c_str());
	std::time_t time = std::time(nullptr);
	std::string currentTimeString = ctime(&time);
	currentTimeString.erase(std::remove(currentTimeString.begin(), currentTimeString.end(), '\n'),
							currentTimeString.end()); //Remove the trailing newline character
	newBackup->InsertNewChildElement("backupDate")->InsertNewText(currentTimeString.c_str());
	newBackup->FirstChildElement("backupDate")->SetAttribute("time", std::time(nullptr));
	rootNode->LinkEndChild(newBackup);
	document.SaveFile(vaultPath.string().c_str());
}

//Find the first ID in XML which is not in use
int VaultManager::firstAvailableID() {
	int id = 0;
	for(; parseFile(id) != nullptr; id++); //Look for any file with this ID
	return id;
}