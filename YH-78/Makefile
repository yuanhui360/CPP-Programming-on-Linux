CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lpthread -lrt

all: mqueue_recv mqueue_send mqueue_notice

mqueue_recv: mqueue_recv.o
    $(CC) $(LDFLAGS) mqueue_recv.o -o mqueue_recv
mqueue_recv.o: mqueue_recv.cpp
    $(CC) $(CFLAGS) mqueue_recv.cpp

mqueue_send: mqueue_send.o
    $(CC) $(LDFLAGS) mqueue_send.o -o mqueue_send
mqueue_send.o: mqueue_send.cpp
    $(CC) $(CFLAGS) mqueue_send.cpp

mqueue_notice: mqueue_notice.o
    $(CC) $(LDFLAGS) mqueue_notice.o -o mqueue_notice
mqueue_notice.o: mqueue_notice.cpp
    $(CC) $(CFLAGS) mqueue_notice.cpp

clean:
    rm -f *.o mqueue_recv mqueue_send mqueue_notice
