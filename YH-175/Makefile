#
# Hui test g++ Makefile on Linux
# Note : It may require to replace front space characters with <TAB> after downloaded this Makefile
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -std=c++17 -lpthread -lev

all: libev_CPP_stat_wrapper

libev_CPP_stat_wrapper: libev_CPP_stat_wrapper.o
    $(CC) libev_CPP_stat_wrapper.o $(LDFLAGS) -o libev_CPP_stat_wrapper
libev_CPP_stat_wrapper.o: libev_CPP_stat_wrapper.cpp
    $(CC) $(CFLAGS) libev_CPP_stat_wrapper.cpp $(LDFLAGS) -o libev_CPP_stat_wrapper.o

clean:
    rm -f *.o libev_CPP_stat_wrapper
