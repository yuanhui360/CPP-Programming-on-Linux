CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lssh -pthread -L /usr/local/lib/x86_64-linux-gnu -lrabbitmq -lamqpcpp

all: read_queue headers_publish

read_queue: read_queue.o
	$(CC) read_queue.o -o read_queue $(LDFLAGS)
read_queue.o: read_queue.cpp
	$(CC) $(CFLAGS) read_queue.cpp -o read_queue.o

headers_publish: headers_publish.o
	$(CC) headers_publish.o -o headers_publish $(LDFLAGS)
headers_publish.o: headers_publish.cpp
	$(CC) $(CFLAGS) headers_publish.cpp -o headers_publish.o

clean:
	rm -f *.o read_queue headers_publish
