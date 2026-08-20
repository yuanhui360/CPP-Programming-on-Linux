#
# Hui test g++ Makefile on Linux
# Note: It may require to replace fron space characters with <TAB> after downloaded this file
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -std=c++17 -lamqpcpp -lrabbitmq -lpthread -ldl -levent -lssl

all: AMQP-CPP_Publish AMQP-CPP_check_queue AMQP-CPP_Consume AMQP-CPP_Get

AMQP-CPP_Consume: AMQP-CPP_Consume.o
    $(CC) AMQP-CPP_Consume.o $(LDFLAGS) -o AMQP-CPP_Consume
AMQP-CPP_Consume.o: AMQP-CPP_Consume.cpp
    $(CC) $(CFLAGS) AMQP-CPP_Consume.cpp $(LDFLAGS) -o AMQP-CPP_Consume.o

AMQP-CPP_Publish: AMQP-CPP_Publish.o
    $(CC) AMQP-CPP_Publish.o $(LDFLAGS) -o AMQP-CPP_Publish
AMQP-CPP_Publish.o: AMQP-CPP_Publish.cpp
    $(CC) $(CFLAGS) AMQP-CPP_Publish.cpp $(LDFLAGS) -o AMQP-CPP_Publish.o

AMQP-CPP_check_queue: AMQP-CPP_check_queue.o
    $(CC) AMQP-CPP_check_queue.o $(LDFLAGS) -o AMQP-CPP_check_queue
AMQP-CPP_check_queue.o: AMQP-CPP_check_queue.cpp
    $(CC) $(CFLAGS) AMQP-CPP_check_queue.cpp $(LDFLAGS) -o AMQP-CPP_check_queue.o

AMQP-CPP_Get: AMQP-CPP_Get.o
    $(CC) AMQP-CPP_Get.o $(LDFLAGS) -o AMQP-CPP_Get
AMQP-CPP_Get.o: AMQP-CPP_Get.cpp
    $(CC) $(CFLAGS) AMQP-CPP_Get.cpp $(LDFLAGS) -o AMQP-CPP_Get.o

clean:
    rm -f *.o AMQP-CPP_Publish AMQP-CPP_check_queue AMQP-CPP_Consume AMQP-CPP_Get
