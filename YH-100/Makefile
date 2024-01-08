CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lrt -lpthread -lboost_system -std=c++11

all: create_shared_mem open_shared_mem

create_shared_mem: create_shared_mem.o
	$(CC) $(LDFLAGS) create_shared_mem.o -o create_shared_mem
create_shared_mem.o: create_shared_mem.cpp
	$(CC) $(CFLAGS) create_shared_mem.cpp $(LDFLAGS)

open_shared_mem: open_shared_mem.o
	$(CC) $(LDFLAGS) open_shared_mem.o -o open_shared_mem
open_shared_mem.o: open_shared_mem.cpp
	$(CC) $(CFLAGS) open_shared_mem.cpp $(LDFLAGS)

clean:
	rm -f *.o create_shared_mem open_shared_mem
