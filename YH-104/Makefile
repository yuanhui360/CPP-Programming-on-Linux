CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lrt -lpthread -std=c++11

all: pshm_create pshm_open

pshm_create: pshm_create.o
    $(CC) $(LDFLAGS) pshm_create.o -o pshm_create
pshm_create.o: pshm_create.cpp
    $(CC) $(CFLAGS) pshm_create.cpp -o pshm_create.o

pshm_open: pshm_open.o
    $(CC) $(LDFLAGS) pshm_open.o -o pshm_open
pshm_open.o: pshm_open.cpp
    $(CC) $(CFLAGS) pshm_open.cpp -o pshm_open.o

clean:
    rm -f *.o pshm_create pshm_open
