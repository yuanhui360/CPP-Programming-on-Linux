CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lssh -pthread -L /usr/local/lib/x86_64-linux-gnu -lrabbitmq -lamqpcpp

all: my_consume  my_get  my_publish

my_consume: my_consume.o
    $(CC) $(LDFLAGS) my_consume.o -o my_consume
my_consume.o: my_consume.cpp
    $(CC) $(CFLAGS) my_consume.cpp -o my_consume.o

my_get: my_get.o
    $(CC) $(LDFLAGS) my_get.o -o my_get
my_get.o: my_get.cpp
    $(CC) $(CFLAGS) my_get.cpp -o my_get.o

my_publish: my_publish.o
    $(CC) $(LDFLAGS) my_publish.o -o my_publish
my_publish.o: my_publish.cpp
    $(CC) $(CFLAGS) my_publish.cpp -o my_publish.o

clean:
    rm -f *.o my_consume  my_get  my_publish
