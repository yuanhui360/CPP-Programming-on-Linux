#
# Hui test g++ Makefile on Linux
# Note : it may require to replace front spece characters with <TAB> after you downloaded this file
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -std=c++17 -lpthread -lev

all: my_basic_pthread libev_ev_async_c libev_ev_async_cpp

my_basic_pthread: my_basic_pthread.o
    $(CC) my_basic_pthread.o $(LDFLAGS) -o my_basic_pthread
my_basic_pthread.o: my_basic_pthread.cpp
    $(CC) $(CFLAGS) my_basic_pthread.cpp $(LDFLAGS) -o my_basic_pthread.o

libev_ev_async_c: libev_ev_async_c.o
    $(CC) libev_ev_async_c.o $(LDFLAGS) -o libev_ev_async_c
libev_ev_async_c.o: libev_ev_async_c.cpp
    $(CC) $(CFLAGS) libev_ev_async_c.cpp $(LDFLAGS) -o libev_ev_async_c.o

libev_ev_async_cpp: libev_ev_async_cpp.o
    $(CC) libev_ev_async_cpp.o $(LDFLAGS) -o libev_ev_async_cpp
libev_ev_async_cpp.o: libev_ev_async_cpp.cpp
    $(CC) $(CFLAGS) libev_ev_async_cpp.cpp $(LDFLAGS) -o libev_ev_async_cpp.o

clean:
    rm -f *.o my_basic_pthread libev_ev_async_c libev_ev_async_cpp
