#include <fstream>
#include <iostream>
#include <String>
#include <filesystem>
#include <cstdio>

//A class which will handle a vault folder
    //Making sure the vault has all required files
    //Retrievs files and manages the files in the vault
class VaultManager
{

    public:
        template <typename T>
        void fileRetrieve(T identifier)
        {
            if (!std::is_same(T, std::string) || !std::is_same(T, int))
            {
                cout << "Invalid format." << endl;
                return 1;
            }
            if (std::is_same(T, std::string))
            {
                //Identifier is string
            } else
            {
                //Identifier is integer
            }
        }

};

//A class for reading and writing configuration files
class ConfigManager
{
    private:
        std::filesystem::path configFilePath = std::filesystem::path("configuration.conf");

    public:
        enum confOption
        {
            vaultPath
        };
        
        void setConfig(confOption setting, std::string value)
        {
            //Check if configuration file exists
            if (!std::filesystem::exists(configFilePath))
            {
                std::cout << "Warning: Configuration file not found.\nCreating new configuration file.";
                if (setting != confOption::vaultPath)
                {
                    //Set default vaultPath
                    std::ofstream configFile = std::ofstream(configFilePath);
                    
                }
                else
                {

                }
            }
        }
        template <typename T>
        T getConfig()
        {

        }
};

class FileManager
{
    private:
        char copyBuffer[32000];
        std::streamsize copyBufferSize = 32000;
        std::ifstream sourceFile;
        std::ofstream targetFile;
        ConfigManager ConfManager = ConfigManager();

    public:
        //Todo: Make functions return success/ failure



        void fileBackup(std::string source, std::string target)
        {
            sourceFile.open(source, std::ios_base::binary);
            targetFile.open(target, std::ios_base::binary);

            do {
                sourceFile.read(copyBuffer, copyBufferSize);
                targetFile.write(copyBuffer, copyBufferSize);
            } while (!sourceFile.eof());

            sourceFile.close();
            targetFile.close();
        }



        void setBackupDestination(std::string path)
        {
            //First check if target path already has necessary folder
            if (std::filesystem::exists(path + "\\backupVault"))
            {
                ConfManager.setConfig(ConfManager.confOption::vaultPath, path + "\\backupVault");
                std::cout << "Vault already exists at this location.\nFile storage configuration updated.";
            }
            else
            {
                //Create the vault at this location
                std::filesystem::create_directory(path + "\\backupVault");
                std::cout << "Vault created at location.\nFile storage configuration updated.";
            }


        }
};

int main(int argc, char *argv[])
{
    FileManager manager;
    manager.fileBackup(argv[1], argv[2]);

    return 0;
}