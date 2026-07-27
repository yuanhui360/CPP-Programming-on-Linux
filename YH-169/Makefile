#
# Hui test g++ Makefile on Linux
#
# Note: It might require to replace front space characters with <TAB> after you downloaded this file
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lamqpcpp -lrabbitmq -lssl -lcrypto

all: amqpcpp_ssl_direct_publish  amqpcpp_ssl_get

amqpcpp_ssl_get: amqpcpp_ssl_get.o
    $(CC) amqpcpp_ssl_get.o $(LDFLAGS) -o amqpcpp_ssl_get
amqpcpp_ssl_get.o: amqpcpp_ssl_get.cpp
    $(CC) $(CFLAGS) amqpcpp_ssl_get.cpp $(LDFLAGS) -o amqpcpp_ssl_get.o

amqpcpp_ssl_direct_publish: amqpcpp_ssl_direct_publish.o
    $(CC) amqpcpp_ssl_direct_publish.o -o amqpcpp_ssl_direct_publish $(LDFLAGS)
amqpcpp_ssl_direct_publish.o: amqpcpp_ssl_direct_publish.cpp
    $(CC) $(CFLAGS) amqpcpp_ssl_direct_publish.cpp -o amqpcpp_ssl_direct_publish.o $(LDFLAGS)

clean:
    rm -f *.o amqpcpp_ssl_direct_publish  amqpcpp_ssl_get
