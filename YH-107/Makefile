CC=g++
CFLAGS=-c -Wall -g
LDFLAGS =  -lpthread -std=c++11

all: myThreadReadFile myThreadCalculate

myThreadReadFile: myThreadReadFile.o
    $(CC) $(LDFLAGS) myThreadReadFile.o -o myThreadReadFile
myThreadReadFile.o: myThreadReadFile.cpp
    $(CC) $(CFLAGS) myThreadReadFile.cpp -o myThreadReadFile.o

myThreadCalculate: myThreadCalculate.o
    $(CC) $(LDFLAGS) myThreadCalculate.o -o myThreadCalculate
myThreadCalculate.o: myThreadCalculate.cpp
    $(CC) $(CFLAGS) myThreadCalculate.cpp -o myThreadCalculate.o

clean:
    rm -f *.o myThreadReadFile myThreadCalculate
