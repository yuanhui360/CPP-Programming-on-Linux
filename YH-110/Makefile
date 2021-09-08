CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lssh -pthread -L /usr/local/lib/x86_64-linux-gnu -lrabbitmq -lamqpcpp

all: direct_publish fanout_publish topic_publish read_queue

read_queue: read_queue.o
    $(CC) $(LDFLAGS) read_queue.o -o read_queue
read_queue.o: read_queue.cpp
    $(CC) $(CFLAGS) read_queue.cpp -o read_queue.o

direct_publish: direct_publish.o
    $(CC) $(LDFLAGS) direct_publish.o -o direct_publish
direct_publish.o: direct_publish.cpp
    $(CC) $(CFLAGS) direct_publish.cpp -o direct_publish.o

fanout_publish: fanout_publish.o
    $(CC) $(LDFLAGS) fanout_publish.o -o fanout_publish
fanout_publish.o: fanout_publish.cpp
    $(CC) $(CFLAGS) fanout_publish.cpp -o fanout_publish.o

topic_publish: topic_publish.o
    $(CC) $(LDFLAGS) topic_publish.o -o topic_publish
topic_publish.o: topic_publish.cpp
    $(CC) $(CFLAGS) topic_publish.cpp -o topic_publish.o

clean:
    rm -f *.o read_queue direct_publish fanout_publish topic_publish
