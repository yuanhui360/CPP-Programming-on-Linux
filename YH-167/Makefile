#
# Hui test g++ Makefile on Linux
# Note:
#    It requires to replace front speces characters to <TAB> character after downloaded this Makefile
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lamqpcpp -lrabbitmq -lssl -lcrypto

all: rabbitmq_ssl_publish rabbitmq_ssl_get rabbitmq_ssl_queue_check

rabbitmq_ssl_publish: rabbitmq_ssl_publish.o
    $(CC) rabbitmq_ssl_publish.o -o rabbitmq_ssl_publish $(LDFLAGS)
rabbitmq_ssl_publish.o: rabbitmq_ssl_publish.cpp
    $(CC) $(CFLAGS) rabbitmq_ssl_publish.cpp -o rabbitmq_ssl_publish.o $(LDFLAGS)

rabbitmq_ssl_get: rabbitmq_ssl_get.o
    $(CC) rabbitmq_ssl_get.o -o rabbitmq_ssl_get $(LDFLAGS)
rabbitmq_ssl_get.o: rabbitmq_ssl_get.cpp
    $(CC) $(CFLAGS) rabbitmq_ssl_get.cpp -o rabbitmq_ssl_get.o $(LDFLAGS)

rabbitmq_ssl_queue_check: rabbitmq_ssl_queue_check.o
    $(CC) rabbitmq_ssl_queue_check.o -o rabbitmq_ssl_queue_check $(LDFLAGS)
rabbitmq_ssl_queue_check.o: rabbitmq_ssl_queue_check.cpp
    $(CC) $(CFLAGS) rabbitmq_ssl_queue_check.cpp -o rabbitmq_ssl_queue_check.o $(LDFLAGS)

clean:
    rm -f *.o rabbitmq_ssl_publish rabbitmq_ssl_get rabbitmq_ssl_queue_check
