#include <cstdio>
#include <iostream>
#include "includes/backfile.h"

BackFile::BackFile(ConfigManager *configManager, const int idFile)
{
	//Extract and populate the variables
	if (configManager == nullptr)
		return; //TODO: Might be dangerous behaviour
	id = idFile;
	tinyxml2::XMLDocument document;
	document.LoadFile((configManager->getVaultPath() / "vault.xml").string().c_str());
	if (!document.RootElement()->FirstChildElement("file"))
		throw std::exception(); //Make sure that there are files in XML

	tinyxml2::XMLElement *element = document.RootElement()->FirstChildElement("file");
	do
	{
		if (std::stoi(element->FirstChildElement("id")->GetText()) == idFile)
			break;
		else
			element = element->NextSiblingElement();
	} while (element != std::nullptr_t());

	if (element != std::nullptr_t())
	{
		name = element->FirstChildElement("fileName")->GetText();
		backupName = element->FirstChildElement("backupName")->GetText();
		filePath = element->FirstChildElement("filePath")->GetText();
		if (element->FirstChildElement("backupDate")->IntAttribute("time"))
			date = new std::time_t(element->FirstChildElement("backupDate")->IntAttribute("time"));
		else //File is missing date in seconds
		{
			date = nullptr;
			location = filePath.string().erase(filePath.string().length() - filePath.filename().string().length());
			std::cerr << location << std::endl;
		}
	}
}

BackFile::~BackFile() { delete date; }

int BackFile::getID()                           const { return id; }
std::string BackFile::getName()                 const { return name; }
std::string BackFile::getBackupName()           const { return backupName; }
std::filesystem::path BackFile::getPath()       const { return filePath; }
std::filesystem::path BackFile::getLocation()   const { return location; }
std::time_t *BackFile::getDate()                const { return date; }