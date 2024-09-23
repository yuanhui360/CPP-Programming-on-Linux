# It might require to replace the leading space characters with <TAB> when you use this Makefile

CC=g++
CFLAGS=-c -Wall
LDFLAGS = -levent -levent_core -levent_extra -levent_openssl -lcrypto -lssl

all: bev_tcp_echo_server bev_tls_echo_server basic_tcp_client openssl_tls_client

basic_tcp_client: basic_tcp_client.o
    $(CC) basic_tcp_client.o $(LDFLAGS) -o basic_tcp_client
basic_tcp_client.o: basic_tcp_client.cpp
    $(CC) $(CFLAGS) basic_tcp_client.cpp -o  basic_tcp_client.o

openssl_tls_client: openssl_tls_client.o
    $(CC) openssl_tls_client.o $(LDFLAGS) -o openssl_tls_client
openssl_tls_client.o: openssl_tls_client.cpp
    $(CC) $(CFLAGS) openssl_tls_client.cpp -o  openssl_tls_client.o

bev_tls_echo_server: bev_tls_echo_server.o
    $(CC) bev_tls_echo_server.o $(LDFLAGS) -o bev_tls_echo_server
bev_tls_echo_server.o: bev_tls_echo_server.cpp
    $(CC) $(CFLAGS) bev_tls_echo_server.cpp -o  bev_tls_echo_server.o

bev_tcp_echo_server: bev_tcp_echo_server.o
    $(CC) bev_tcp_echo_server.o $(LDFLAGS) -o bev_tcp_echo_server
bev_tcp_echo_server.o: bev_tcp_echo_server.cpp
    $(CC) $(CFLAGS) bev_tcp_echo_server.cpp -o  bev_tcp_echo_server.o

clean:
    rm -f *.o bev_tcp_echo_server bev_tls_echo_server basic_tcp_client openssl_tls_client
