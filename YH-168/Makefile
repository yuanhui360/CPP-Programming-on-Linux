#
# Hui test g++ Makefile on Linux
# Note: It might require to replace front space characters with <TAB> after you downloaded this file
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lamqpcpp -lrabbitmq -lssl -lcrypto

all: amqpcpp_get  amqpcpp_direct_publish

amqpcpp_get: amqpcpp_get.o
    $(CC) amqpcpp_get.o $(LDFLAGS) -o amqpcpp_get
amqpcpp_get.o: amqpcpp_get.cpp
    $(CC) $(CFLAGS) amqpcpp_get.cpp $(LDFLAGS) -o amqpcpp_get.o

amqpcpp_direct_publish: amqpcpp_direct_publish.o
    $(CC) amqpcpp_direct_publish.o -o amqpcpp_direct_publish $(LDFLAGS)
amqpcpp_direct_publish.o: amqpcpp_direct_publish.cpp
    $(CC) $(CFLAGS) amqpcpp_direct_publish.cpp -o amqpcpp_direct_publish.o $(LDFLAGS)

clean:
    rm -f *.o amqpcpp_get amqpcpp_direct_publish
