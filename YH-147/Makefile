#
# Hui test g++ Makefile on Linux
# It might need to replace heading <space> to <TAB> character after you downloaded this Makefile
#

CC=g++
CFLAGS=-c -Wall
LDFLAGS = -levent -levent_core -levent_extra -levent_openssl -lcrypto -lssl


all: evhttp_http_server evhttp_http_client evhttp_http_proxy_server

evhttp_http_client: evhttp_http_client.o
    $(CC) evhttp_http_client.o $(LDFLAGS) -o evhttp_http_client
evhttp_http_client.o: evhttp_http_client.cpp
    $(CC) $(CFLAGS) evhttp_http_client.cpp -o  evhttp_http_client.o

evhttp_http_server: evhttp_http_server.o
    $(CC) evhttp_http_server.o $(LDFLAGS) -o evhttp_http_server
evhttp_http_server.o: evhttp_http_server.cpp
    $(CC) $(CFLAGS) evhttp_http_server.cpp -o  evhttp_http_server.o

evhttp_http_proxy_server: evhttp_http_proxy_server.o
    $(CC) evhttp_http_proxy_server.o $(LDFLAGS) -o evhttp_http_proxy_server
evhttp_http_proxy_server.o: evhttp_http_proxy_server.cpp
    $(CC) $(CFLAGS) evhttp_http_proxy_server.cpp -o  evhttp_http_proxy_server.o

clean:
    rm -f *.o evhttp_http_server evhttp_http_client evhttp_http_proxy_server
