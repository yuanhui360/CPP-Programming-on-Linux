#
# Hui test g++ Makefile on Linux
#

CC=g++
CFLAGS=-c -Wall
LDFLAGS = -levent -levent_core -levent_extra -levent_openssl -lcrypto -lssl -lboost_system -lboost_iostreams

all: bev_tcp_echo_server bev_tcp_proxy_server  basic_tcp_client

bev_tcp_echo_server: bev_tcp_echo_server.o
    $(CC) bev_tcp_echo_server.o $(LDFLAGS) -o bev_tcp_echo_server
bev_tcp_echo_server.o: bev_tcp_echo_server.cpp

bev_tcp_proxy_server: bev_tcp_proxy_server.o
    $(CC) bev_tcp_proxy_server.o $(LDFLAGS) -o bev_tcp_proxy_server
bev_tcp_proxy_server.o: bev_tcp_proxy_server.cpp
    $(CC) $(CFLAGS) bev_tcp_proxy_server.cpp -o  bev_tcp_proxy_server.o

basic_tcp_client: basic_tcp_client.o
    $(CC) basic_tcp_client.o $(LDFLAGS) -o basic_tcp_client
basic_tcp_client.o: basic_tcp_client.cpp
    $(CC) $(CFLAGS) basic_tcp_client.cpp -o  basic_tcp_client.o

clean:
    rm -f *.o bev_tcp_echo_server bev_tcp_proxy_server basic_tcp_client
            
