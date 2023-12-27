CC=g++
CFLAGS=-c -Wall -Werror -g -m64 -D_FILE_OFFSET_BITS=64
LDFLAGS = -lfuse3

all: my_fuse3 my_fuse3_ll my_fuse3_ll_cap

my_fuse3: my_fuse3.o
    $(CC) my_fuse3.o $(LDFLAGS) -o my_fuse3
my_fuse3.o: my_fuse3.cpp
    $(CC) $(CFLAGS) my_fuse3.cpp

my_fuse3_ll: my_fuse3_ll.o
    $(CC) my_fuse3_ll.o $(LDFLAGS) -o my_fuse3_ll
my_fuse3_ll.o: my_fuse3_ll.cpp
    $(CC) $(CFLAGS) my_fuse3_ll.cpp

my_fuse3_ll_cap: my_fuse3_ll_cap.o
    $(CC) my_fuse3_ll_cap.o $(LDFLAGS) -o my_fuse3_ll_cap
my_fuse3_ll_cap.o: my_fuse3_ll_cap.cpp
    $(CC) $(CFLAGS) my_fuse3_ll_cap.cpp

clean:
    rm -f *.o my_fuse3 my_fuse3_ll my_fuse3_ll_cap
