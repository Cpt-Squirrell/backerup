#include "includes\configmanager.h"
#include "includes\vaultmanager.h"
#include "includes\inputmanager.h"
#include <iostream>

int main(int argc, char *argv[])
{
        //Create a new Config-, Vault-, and Input Manager
    InputManager *inputManager = new InputManager(argc, argv);
    ConfigManager *configManager = new ConfigManager(argv[0]);
    VaultManager *vaultManager = new VaultManager(configManager);

        //Get the input option and perform action
    switch (inputManager->getOption())
    {
        case InputManager::Options::backup: vaultManager->fileBackup(inputManager->getArgument()); break;
        case InputManager::Options::retrieve: vaultManager->fileRetrieve(inputManager->getArgument()); break;
		case InputManager::Options::query: vaultManager->fileQuery(inputManager->getArgument()); break;
        default: std::cout << "There was an error. Option matched no available option." << std::endl; break;
    }

    return 0;
}