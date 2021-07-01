CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lm

all: mmap_read mmap_update mmap_create

mmap_read: mmap_read.o
    $(CC) mmap_read.o -o mmap_read $(LDFLAGS)
mmap_read.o: mmap_read.cpp
    $(CC) $(CFLAGS) mmap_read.cpp -o mmap_read.o

mmap_create: mmap_create.o
    $(CC) mmap_create.o -o mmap_create $(LDFLAGS)
mmap_create.o: mmap_create.cpp
    $(CC) $(CFLAGS) mmap_create.cpp -o mmap_create.o

mmap_update: mmap_update.o
    $(CC) mmap_update.o -o mmap_update $(LDFLAGS)
mmap_update.o: mmap_update.cpp
    $(CC) $(CFLAGS) mmap_update.cpp -o mmap_update.o

clean:
    rm -f *.o mmap_read mmap_update mmap_create
