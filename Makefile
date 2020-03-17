CC = g++
CFLAGS = -Wall
Includes = -lstdc++
Name = backerup

default: backuper
windows: backuper_windows
win: backuper_windows
backuper: main.cpp backup.a
	$(CC) $(CFLAGS) -o $(Name) main.cpp backup.a $(Includes)

backuper_windows: main.cpp backup.a
	$(CC) $(CFLAGS) -o $(Name).exe main.cpp backup.a $(Includes)

backup.a: backup.o
	ar rvs backup.a backup.o

backup.o: backup.cpp
	$(CC) $(CFLAGS) -c backup.cpp

clean:
	rm backup.a