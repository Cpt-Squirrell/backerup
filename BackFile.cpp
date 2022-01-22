#include <cstdio>
#include <iostream>
#include "includes/backfile.h"

BackFile::BackFile(ConfigManager *configManager, const int idFile)
{
	//Extract and populate the variables
	if (configManager == nullptr)
		return; //TODO: Might be dangerous behaviour
	tinyxml2::XMLDocument document;
	document.LoadFile((configManager->getVaultPath().string() + "vault.xml").c_str());
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
			date = element->FirstChildElement("backupDate")->IntAttribute("time");
		else //File is missing date in seconds
			date = NULL;
		location = filePath.string().erase(filePath.string().length() - filePath.filename().string().length());
		std::cerr << location << std::endl;
	}


}

std::string BackFile::getName() { return name; }
std::string BackFile::getBackupName() { return backupName; }
std::filesystem::path BackFile::getPath() { return filePath; }
std::filesystem::path BackFile::getLocation() { return location; }
std::time_t BackFile::getDate() { return date; }