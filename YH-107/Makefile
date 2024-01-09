CC=g++
CFLAGS=-c -Wall -g
LDFLAGS =  -lpthread -std=c++11

all: myThreadReadFile myThreadCalculate

myThreadReadFile: myThreadReadFile.o
	$(CC) myThreadReadFile.o -o myThreadReadFile $(LDFLAGS)
myThreadReadFile.o: myThreadReadFile.cpp
	$(CC) $(CFLAGS) myThreadReadFile.cpp -o myThreadReadFile.o

myThreadCalculate: myThreadCalculate.o
	$(CC) myThreadCalculate.o -o myThreadCalculate $(LDFLAGS)
myThreadCalculate.o: myThreadCalculate.cpp
	$(CC) $(CFLAGS) myThreadCalculate.cpp -o myThreadCalculate.o

clean:
	rm -f *.o myThreadReadFile myThreadCalculate
