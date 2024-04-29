CC=g++
CFLAGS=-c -Wall
LDFLAGS = -levent -levent_core -levent_extra


all: my_event_client my_block_client my_event_server

my_event_client: my_event_client.o
    $(CC) my_event_client.o $(LDFLAGS) -o my_event_client
my_event_client.o: my_event_client.cpp
    $(CC) $(CFLAGS) my_event_client.cpp -o  my_event_client.o

my_block_client: my_block_client.o
    $(CC) my_block_client.o $(LDFLAGS) -o my_block_client
my_block_client.o: my_block_client.cpp
    $(CC) $(CFLAGS) my_block_client.cpp -o  my_block_client.o

my_event_server: my_event_server.o
    $(CC) my_event_server.o $(LDFLAGS) -o my_event_server
my_event_server.o: my_event_server.cpp
    $(CC) $(CFLAGS) my_event_server.cpp -o  my_event_server.o

clean:
    rm -f *.o my_event_client my_block_client my_event_server
