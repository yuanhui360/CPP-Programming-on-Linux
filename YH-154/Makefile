#
# Hui test g++ Makefile on Linux
#
# Note: It might require to replace front space characters with <TAB> character
#

CC=g++
CFLAGS=-c -Wall
LDFLAGS = -levent -levent_core -levent_extra -lcrypto -lssl -lwolfssl -lpthread

all: openssl_tls_client openssl_tls_server openssl_tls_non_blocking_client

openssl_tls_client: openssl_tls_client.o
    $(CC) openssl_tls_client.o $(LDFLAGS) -o openssl_tls_client
openssl_tls_client.o: openssl_tls_client.cpp
    $(CC) $(CFLAGS) openssl_tls_client.cpp -o  openssl_tls_client.o

openssl_tls_server: openssl_tls_server.o
    $(CC) openssl_tls_server.o $(LDFLAGS) -o openssl_tls_server
openssl_tls_server.o: openssl_tls_server.cpp
    $(CC) $(CFLAGS) openssl_tls_server.cpp -o  openssl_tls_server.o

openssl_tls_non_blocking_client: openssl_tls_non_blocking_client.o
    $(CC) openssl_tls_non_blocking_client.o $(LDFLAGS) -o openssl_tls_non_blocking_client
openssl_tls_non_blocking_client.o: openssl_tls_non_blocking_client.cpp
    $(CC) $(CFLAGS) openssl_tls_non_blocking_client.cpp -o  openssl_tls_non_blocking_client.o

clean:
    rm -f *.o openssl_tls_client openssl_tls_server openssl_tls_non_blocking_client
