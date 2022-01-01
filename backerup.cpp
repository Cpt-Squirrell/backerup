#include "includes\configmanager.h"
#include "includes\vaultmanager.h"
#include "includes\inputmanager.h"

int main(int argc, char *argv[])
{
        //Create a new Config-, Vault-, and Input Manager
    InputManager *inputManager = new InputManager(argc, argv);
    ConfigManager *configManager = new ConfigManager();
    VaultManager *vaultManager = new VaultManager(configManager);

        //Get the input option and perform action
    switch (inputManager->getOption())
    {
        case InputManager::Options::backup: vaultManager->fileBackup(inputManager->getArgument());
        case InputManager::Options::retrieve: vaultManager->fileRetrieve(inputManager->getArgument());
    }

    return 0;
}