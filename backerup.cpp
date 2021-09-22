#include <fstream>
#include <iostream>
#include <String>
#include <filesystem>
#include <cstdio>

int main(int argc, char *argv[])
{
    std::filebuf sourceBuf;
    std::filebuf destinationBuf;
    std::streambuf &destStreamBuf = destinationBuf;
    sourceBuf.open("test.txt", std::ios_base::out);
    destinationBuf.open("test2.txt", std::ios_base::in);
    std::istream sourceFile(&sourceBuf);
    std::ostream destinationFile(&destinationBuf);
    
    do
    {
        std::ios_base::iostate returnBit = sourceFile >> destStreamBuf;
        destinationFile << sourceBuf;
    } while ()
    
    return 0;
}