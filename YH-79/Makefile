CC=g++
CFLAGS=-c -Wall -g
LDFLAGS =  -lpthread -std=c++11

all: myThreadReadFile

myThreadReadFile: myThreadReadFile.o
	$(CC) $(LDFLAGS) myThreadReadFile.o -o myThreadReadFile
myThreadReadFile.o: myThreadReadFile.cpp
	$(CC) $(CFLAGS) myThreadReadFile.cpp -o myThreadReadFile.o

clean:
	rm -f *.o myThreadReadFile
