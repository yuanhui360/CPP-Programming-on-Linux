CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lrt -lpthread -lboost_system -std=c++11

all: open_shared_vector create_shared_vector

create_shared_vector: create_shared_vector.o
	$(CC) $(LDFLAGS) create_shared_vector.o -o create_shared_vector
create_shared_vector.o: create_shared_vector.cpp
	$(CC) $(CFLAGS) create_shared_vector.cpp $(LDFLAGS)

open_shared_vector: open_shared_vector.o
	$(CC) $(LDFLAGS) open_shared_vector.o -o open_shared_vector
open_shared_vector.o: open_shared_vector.cpp
	$(CC) $(CFLAGS) open_shared_vector.cpp $(LDFLAGS)

clean:
	rm -f *.o open_shared_vector create_shared_vector
