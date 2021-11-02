
CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -pthread -lsqlite3

all: sqlite_read sqlite_create

sqlite_read: sqlite_read.o
    $(CC) $(LDFLAGS) sqlite_read.o -o sqlite_read
sqlite_read.o: sqlite_read.cpp
    $(CC) $(CFLAGS) sqlite_read.cpp -o sqlite_read.o

sqlite_create: sqlite_create.o
    $(CC) $(LDFLAGS) sqlite_create.o -o sqlite_create
sqlite_create.o: sqlite_create.cpp
    $(CC) $(CFLAGS) sqlite_create.cpp -o sqlite_create.o

clean:
    rm -f *.o sqlite_read sqlite_create
