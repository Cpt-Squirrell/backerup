compiler = g++
CFLAGS = -Wall

default: backuper
backuper: main.cpp backup.h backup.a
	$(compiler) $(CFLAGS) -o backuper.exe main.cpp backup.h backup.a

backup.a: backup.cpp
	ar rvs backup.a backup.cpp