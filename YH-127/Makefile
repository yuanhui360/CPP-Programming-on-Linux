
CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcryptopp -lcrypto -lssl -lrt -lpthread -lboost_system

all: rsakey_load rsakey_gen

rsakey_gen: rsakey_gen.o
	$(CC) rsakey_gen.o -o rsakey_gen $(LDFLAGS)
rsakey_gen.o: rsakey_gen.cpp
	$(CC) $(CFLAGS) rsakey_gen.cpp -o rsakey_gen.o

rsakey_load: rsakey_load.o
	$(CC) rsakey_load.o -o rsakey_load $(LDFLAGS)
rsakey_load.o: rsakey_load.cpp
	$(CC) $(CFLAGS) rsakey_load.cpp -o rsakey_load.o

clean:
	rm -f *.o rsakey_load rsakey_gen
