#
# Hui test g++ Makefile on Linux
# Note: the new line start with <TAB> 
#

CC=g++
CFLAGS=-c -Wall
LDFLAGS = -levent -levent_core -levent_extra


all: evhttp_http_server evhttp_http_client

evhttp_http_client: evhttp_http_client.o
    $(CC) evhttp_http_client.o $(LDFLAGS) -o evhttp_http_client
evhttp_http_client.o: evhttp_http_client.cpp
    $(CC) $(CFLAGS) evhttp_http_client.cpp -o  evhttp_http_client.o

evhttp_http_server: evhttp_http_server.o
    $(CC) evhttp_http_server.o $(LDFLAGS) -o evhttp_http_server
evhttp_http_server.o: evhttp_http_server.cpp
    $(CC) $(CFLAGS) evhttp_http_server.cpp -o  evhttp_http_server.o

clean:
    rm -f *.o evhttp_http_server evhttp_http_client
