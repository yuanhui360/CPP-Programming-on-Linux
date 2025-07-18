
# Hui test g++ Makefile on Linux
# Note:
# It might require replace front space characters with <TAB> after you have downloaded this makefile
# For instance:
# wolfssl_tls_client: wolfssl_tls_client.o
# <TAB>$(CC) wolfssl_tls_client.o $(LDFLAGS) -o wolfssl_tls_client  <-- Replace front spaces with <TAB>

CC=g++
CFLAGS=-c -Wall
LDFLAGS = -levent -levent_core -levent_extra -lcrypto -lssl -lwolfssl

all: openssl_tls_client openssl_tls_server wolfssl_tls_server wolfssl_tls_client

wolfssl_tls_client: wolfssl_tls_client.o
    $(CC) wolfssl_tls_client.o $(LDFLAGS) -o wolfssl_tls_client
wolfssl_tls_client.o: wolfssl_tls_client.cpp
    $(CC) $(CFLAGS) wolfssl_tls_client.cpp -o  wolfssl_tls_client.o

wolfssl_tls_server: wolfssl_tls_server.o
    $(CC) wolfssl_tls_server.o $(LDFLAGS) -o wolfssl_tls_server
wolfssl_tls_server.o: wolfssl_tls_server.cpp
    $(CC) $(CFLAGS) wolfssl_tls_server.cpp -o  wolfssl_tls_server.o

openssl_tls_client: openssl_tls_client.o
    $(CC) openssl_tls_client.o $(LDFLAGS) -o openssl_tls_client
openssl_tls_client.o: openssl_tls_client.cpp
    $(CC) $(CFLAGS) openssl_tls_client.cpp -o  openssl_tls_client.o

openssl_tls_server: openssl_tls_server.o
    $(CC) openssl_tls_server.o $(LDFLAGS) -o openssl_tls_server
openssl_tls_server.o: openssl_tls_server.cpp
    $(CC) $(CFLAGS) openssl_tls_server.cpp -o  openssl_tls_server.o

clean:
    rm -f *.o openssl_tls_client openssl_tls_server wolfssl_tls_server wolfssl_tls_client
