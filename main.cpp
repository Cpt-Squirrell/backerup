#include <iostream>
#include <string>
#include "includes/backup.h"

int main (int argc, char *argv[]) {
    std::string launchParameter; if (argc < 2) { launchParameter = argv[2]; }

    //Launch Options
    if (argc < 2) {
        //No parameter assigned. Default to list.
        std::cout << "No parameters passed. Defaulting to 'list'.\nType 'help' for avaliable options.\n";
        readBackups();
    } else if (launchParameter.compare("-l") || launchParameter.compare("list") ) {
        //-l or list. Lists all backed up dir./files.
        readBackups();
    } else if (launchParameter.compare("-b") || launchParameter.compare("backup")) {
        //-b or backup. Selects a dir./file to be backed up.

    } else if (launchParameter.compare("-r") || launchParameter.compare("retrieve")) {
        //-r or retrieve. Retrieves a backed up dir./file.
        
    } else if (launchParameter.compare("-h") || launchParameter.compare("help")) {
        //-h or help. Displays all avaliable options.

    } else { std::cout << "Invalid parameter(s). Use 'help' for options."; }

    return 0;
}