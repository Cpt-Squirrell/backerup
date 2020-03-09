#include <iostream>
#include "backup.h"

int main (int argc, char *argv[]) {

    //Launch Options
    if (argc < 2) {
        //No parameter assigned. Default to list.
        std::cout << "No parameters passed. Defaulting to 'list'.\nType 'help' for avaliable options.\n";
        readBackups();
    } else if (argv[2] == "-l" || argv[2] == "list") {
        //-l or list. Lists all backed up dir./files.
        readBackups();
    } else if (argv[2] == "-b" || argv[2] == "backup") {
        //-b or backup. Selects a dir./file to be backed up.

    } else if (argv[2] == "-r" || argv[2] == "retrieve") {
        //-r or retrieve. Retrieves a backed up dir./file.
        
    } else if (argv[2] == "-h" || argv[2] == "help") {
        //-h or help. Displays all avaliable options.

    } else { std::cout << "Invalid parameter(s). Use 'help' for options."; }

}