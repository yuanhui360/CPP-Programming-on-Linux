#
# Hui test g++ Makefile on Linux
#

CC=g++
CFLAGS=-c -Wall
LDFLAGS = -levent -levent_core -levent_extra  -lcrypto -lssl

all: openssl_tls_server openssl_tls_client basic_tcp_client basic_tcp_server

basic_tcp_client: basic_tcp_client.o
    $(CC) basic_tcp_client.o $(LDFLAGS) -o basic_tcp_client
basic_tcp_client.o: basic_tcp_client.cpp
    $(CC) $(CFLAGS) basic_tcp_client.cpp -o  basic_tcp_client.o

basic_tcp_server: basic_tcp_server.o
    $(CC) basic_tcp_server.o $(LDFLAGS) -o basic_tcp_server
basic_tcp_server.o: basic_tcp_server.cpp
    $(CC) $(CFLAGS) basic_tcp_server.cpp -o  basic_tcp_server.o

openssl_tls_client: openssl_tls_client.o
    $(CC) openssl_tls_client.o $(LDFLAGS) -o openssl_tls_client
openssl_tls_client.o: openssl_tls_client.cpp
    $(CC) $(CFLAGS) openssl_tls_client.cpp -o  openssl_tls_client.o

openssl_tls_server: openssl_tls_server.o
    $(CC) openssl_tls_server.o $(LDFLAGS) -o openssl_tls_server
openssl_tls_server.o: openssl_tls_server.cpp
    $(CC) $(CFLAGS) openssl_tls_server.cpp -o  openssl_tls_server.o

clean:
    rm -f *.o openssl_tls_server openssl_tls_client basic_tcp_client basic_tcp_server
