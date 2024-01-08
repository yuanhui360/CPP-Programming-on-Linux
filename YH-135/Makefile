CC=g++
CFLAGS=-c -Wall -Werror -g -m64 -D_FILE_OFFSET_BITS=64
LDFLAGS = -lfuse -lfuse3

all: my_fuse

my_fuse: my_fuse.o
	$(CC) my_fuse.o $(LDFLAGS) -o my_fuse
my_fuse.o: my_fuse.cpp
	$(CC) $(CFLAGS) my_fuse.cpp

clean:
	rm -f *.o my_fuse
