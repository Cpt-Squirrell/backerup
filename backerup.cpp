#include "includes/configmanager.h"
#include "includes/vaultmanager.h"
#include "includes/inputmanager.h"
#include <iostream>

void help() {
	std::cout << "Backerup is a very simple CLI backup utility.\n"
			  << "Backup location is dubbed the 'vault'.\n"
			  << "See list of commands: \n"
			  << "\tbackup (file | file path)\n"
			  << "\t\tWill place the given file into the vault.\n"
			  << "\tretrieve (file)\n"
			  << "\t\tWill attempt to get given file. \n\t\tName must be exact. See query or list for exact name.\n"
			  << "\tquery (name)\n"
			  << "\t\tWill do a quick search for files with given name.\n"
			  << "\tlist\n"
			  << "\t\tWill list every file in the vault.\n"
			  << "\thelp\n"
			  << "\t\tThis.\n";
}

int main(int argc, char *argv[]) {
	//Create a new Config-, Vault-, and Input Manager
	auto *inputManager = new InputManager(argc, argv);
	auto *configManager = new ConfigManager(argv[0]);
	auto *vaultManager = new VaultManager(configManager);

	//Get the input option and perform action
	switch (inputManager->getOption()) {
		case InputManager::Options::backup:
			vaultManager->fileBackup(inputManager->getArgument());
			break;
		case InputManager::Options::retrieve:
			vaultManager->fileRetrieve(inputManager->getArgument());
			break;
		case InputManager::Options::restore:
			vaultManager->fileRestore(inputManager->getArgument());
			break;
		case InputManager::Options::query:
			vaultManager->fileQuery(inputManager->getArgument());
			break;
		case InputManager::Options::list:
			vaultManager->listFiles();
			break;
		case InputManager::Options::help:
			help();
			break;
		default:
			std::cout << "There was an error. Option matched no available option. See help menu." << std::endl;
			break;
	}


	return 0;
}