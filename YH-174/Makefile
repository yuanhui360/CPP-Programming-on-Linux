#
# Hui test g++ Makefile on Linux
# Note : It may require to replace fron space characters with <TAB> after you downloaded this Makefile
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lpthread -lev

all: libev_ev_stat

libev_ev_stat: libev_ev_stat.o
    $(CC) libev_ev_stat.o $(LDFLAGS) -o libev_ev_stat
libev_ev_stat.o: libev_ev_stat.cpp
    $(CC) $(CFLAGS) libev_ev_stat.cpp $(LDFLAGS) -o libev_ev_stat.o

clean:
    rm -f *.o libev_ev_stat
