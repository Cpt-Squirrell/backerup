#include "includes/backup.h"
#include <fstream>

//For debugging \/
#include <iostream>

void readBackups () {
    std::cout << "readBackups();\n";
    char readByte[128];
    std::fstream backups;
    backups.open("backups.txt", std::fstream::in);
        backups.getline(readByte, 64);
    backups.close();
}