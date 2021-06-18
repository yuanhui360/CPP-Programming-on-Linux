CC=g++
CFLAGS=-c -Wall -g -I./
LDFLAGS = -lxerces-c-3.2 -std=c++11

all: MyDOMLSParser

MyDOMLSParser: MyDOMLSParser.o DOMPrintFilter.o
    $(CC) MyDOMLSParser.o DOMPrintFilter.o -o MyDOMLSParser $(LDFLAGS)
MyDOMLSParser.o: MyDOMLSParser.cpp
    $(CC) $(CFLAGS) MyDOMLSParser.cpp -o MyDOMLSParser.o
DOMPrintFilter.o: DOMPrintFilter.cpp
    $(CC) $(CFLAGS) DOMPrintFilter.cpp -o DOMPrintFilter.o

clean:
    rm -f *.o MyDOMLSParser
