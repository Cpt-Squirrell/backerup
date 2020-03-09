#include "backup.h"
#include <fstream>

void readBackups () {
    char readByte[32];
    std::fstream backups;
    backups.open("backups.txt", std::fstream::in);
    backups.read(readByte, backups.gcount());
    backups.close();
}