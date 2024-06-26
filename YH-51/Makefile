CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lboost_regex -lboost_iostreams -lpthread -lboost_system

all: udp_client udp_server

udp_client: udp_client.o
	$(CC) udp_client.o -o udp_client $(LDFLAGS)
udp_client.o: udp_client.cpp
	$(CC) $(CFLAGS) udp_client.cpp  $(LDFLAGS)

udp_server: udp_server.o
	$(CC) udp_server.o -o udp_server $(LDFLAGS)
udp_server.o: udp_server.cpp
	$(CC) $(CFLAGS) udp_server.cpp  $(LDFLAGS)

clean:
	rm -f *.o udp_client udp_server
