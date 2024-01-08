CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lboost_regex -lboost_iostreams -lpthread -lboost_system -lcrypto -lssl -std=c++11

all: ssl_server ssl_client

ssl_client: ssl_client.o client_main.o
	$(CC) $(LDFLAGS) ssl_client.o client_main.o -o ssl_client
ssl_client.o: ssl_client.cpp
	$(CC) $(CFLAGS) ssl_client.cpp $(LDFLAGS)
client_main.o: client_main.cpp
	$(CC) $(CFLAGS) client_main.cpp $(LDFLAGS)

ssl_server: ssl_server.o
	$(CC) $(LDFLAGS) ssl_server.o -o ssl_server
ssl_server.o: ssl_server.cpp
	$(CC) $(CFLAGS) ssl_server.cpp $(LDFLAGS)

clean:
	rm -f *.o ssl_server ssl_client
