CC=g++
CFLAGS=-c -Wall -g
LDFLAGS =  -lpq  -lpqxx -lssh

all: pqxx_select_ex

pqxx_select_ex: pqxx_select_ex.o
	$(CC) pqxx_select_ex.o -o pqxx_select_ex -std=c++11 $(LDFLAGS)
pqxx_select_ex.o: pqxx_select_ex.cpp
	$(CC) $(CFLAGS) pqxx_select_ex.cpp -o pqxx_select_ex.o

clean:
	rm -f *.o pqxx_select_ex
