
CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -pthread -L /usr/local/lib/x86_64-linux-gnu -lrabbitmq

all: amqp_publish amqp_get_message amqp_consumer

amqp_publish: amqp_publish.o
	$(CC) $(LDFLAGS) amqp_publish.o -o amqp_publish
amqp_publish.o: amqp_publish.cpp
	$(CC) $(CFLAGS) amqp_publish.cpp -o amqp_publish.o

amqp_consumer: amqp_consumer.o
	$(CC) $(LDFLAGS) amqp_consumer.o -o amqp_consumer
amqp_consumer.o: amqp_consumer.cpp
	$(CC) $(CFLAGS) amqp_consumer.cpp -o amqp_consumer.o

amqp_get_message: amqp_get_message.o
	$(CC) $(LDFLAGS) amqp_get_message.o -o amqp_get_message
amqp_get_message.o: amqp_get_message.cpp
	$(CC) $(CFLAGS) amqp_get_message.cpp -o amqp_get_message.o

clean:
	rm -f *.o amqp_publish amqp_get_message amqp_consumer
