CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lrt -lpthread -lboost_system -std=c++11

all: create_shared_memory_object open_shared_memory_object

create_anonymous_mutex: create_anonymous_mutex.o
	$(CC) $(LDFLAGS) create_shared_memory_object.o -o create_shared_memory_object
create_shared_memory_object.o: create_shared_memory_object.cpp
	$(CC) $(CFLAGS) create_shared_memory_object.cpp $(LDFLAGS)

open_shared_memory_object: open_shared_memory_object.o
	$(CC) $(LDFLAGS) open_shared_memory_object.o -o open_shared_memory_object
open_shared_memory_object.o: open_shared_memory_object.cpp
	$(CC) $(CFLAGS) open_shared_memory_object.cpp $(LDFLAGS)

clean:
	rm -f *.o create_shared_memory_object open_shared_memory_object
