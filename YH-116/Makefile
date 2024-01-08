CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -pthread -lsqlite3

all: sqlite_vector sqlite_vector

sqlite_basic: sqlite_basic.o
	$(CC) $(LDFLAGS) sqlite_basic.o -o sqlite_basic
sqlite_basic.o: sqlite_basic.cpp
	$(CC) $(CFLAGS) sqlite_basic.cpp -o sqlite_basic.o

sqlite_vector: sqlite_vector.o  sqlite_class.o
	$(CC) $(LDFLAGS) sqlite_vector.o sqlite_class.o -o sqlite_vector
sqlite_vector.o: sqlite_vector.cpp
	$(CC) $(CFLAGS) sqlite_vector.cpp -o sqlite_vector.o
sqlite_class.o: sqlite_class.cpp
	$(CC) $(CFLAGS) sqlite_class.cpp -o sqlite_class.o

clean:
	rm -f *.o sqlite_basic sqlite_vector
