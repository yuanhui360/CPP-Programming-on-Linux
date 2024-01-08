CC=g++
CFLAGS=-c -Wall -g -I./
LDFLAGS = -lxerces-c-3.2 -std=c++11

all: MySAXParser

MySAXParser: MySAXParser.o MySAXHandler.o
	$(CC) MySAXParser.o MySAXHandler.o $(LDFLAGS) -o MySAXParser
MySAXParser.o: MySAXParser.cpp
	$(CC) $(CFLAGS) MySAXParser.cpp -o MySAXParser.o
MySAXHandler.o: MySAXHandler.cpp
	$(CC) $(CFLAGS) MySAXHandler.cpp -o MySAXHandler.o

clean:
	rm -f *.o MySAXParser
