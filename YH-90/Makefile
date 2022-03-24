CC=g++
CFLAGS=-c -Wall -g -I./
LDFLAGS = -lxerces-c-3.2 -std=c++11

all: MyDOMView MyDOMDCreat MyDOMChange

MyDOMChange: MyDOMChange.o
    $(CC) MyDOMChange.o $(LDFLAGS) -o MyDOMChange
MyDOMChange.o: MyDOMChange.cpp
    $(CC) $(CFLAGS) MyDOMChange.cpp -o MyDOMChange.o

MyDOMView: MyDOMView.o
    $(CC) MyDOMView.o $(LDFLAGS) -o MyDOMView
MyDOMView.o: MyDOMView.cpp
    $(CC) $(CFLAGS) MyDOMView.cpp -o MyDOMView.o

MyDOMDCreat: MyDOMDCreat.o
    $(CC) $(LDFLAGS) MyDOMDCreat.o -o MyDOMDCreat
MyDOMDCreat.o: MyDOMDCreat.cpp
    $(CC) $(CFLAGS) MyDOMDCreat.cpp -o MyDOMDCreat.o

clean:
    rm -f *.o MyDOMView MyDOMDCreat
