#
# Hui test g++ Makefile on Linux
# Note:
#   After downloaded this Makefile, it requires to replace some front space charters with <TAB> character
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lamqpcpp -lrabbitmq -lssl -lcrypto

all: rabbitmq_publish rabbitmq_get rabbitmq_queue_check 

rabbitmq_publish: rabbitmq_publish.o
    $(CC) rabbitmq_publish.o -o rabbitmq_publish $(LDFLAGS)
rabbitmq_publish.o: rabbitmq_publish.cpp
    $(CC) $(CFLAGS) rabbitmq_publish.cpp -o rabbitmq_publish.o $(LDFLAGS)

rabbitmq_get: rabbitmq_get.o
    $(CC) rabbitmq_get.o -o rabbitmq_get $(LDFLAGS)
rabbitmq_get.o: rabbitmq_get.cpp
    $(CC) $(CFLAGS) rabbitmq_get.cpp -o rabbitmq_get.o $(LDFLAGS)

rabbitmq_queue_check: rabbitmq_queue_check.o
    $(CC) rabbitmq_queue_check.o -o rabbitmq_queue_check $(LDFLAGS)
rabbitmq_queue_check.o: rabbitmq_queue_check.cpp
    $(CC) $(CFLAGS) rabbitmq_queue_check.cpp -o rabbitmq_queue_check.o $(LDFLAGS)

clean:
    rm -f *.o rabbitmq_publish rabbitmq_get rabbitmq_queue_check
