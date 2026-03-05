# Hui test g++ Makefile on Linux
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lX11 -lssh -lpthread

all: x11_Drawing

x11_Drawing: x11_Drawing.o
    $(CC) x11_Drawing.o -o x11_Drawing $(LDFLAGS)
x11_Drawing.o: x11_Drawing.cpp
    $(CC) $(CFLAGS) x11_Drawing.cpp $(LDFLAGS)

clean:
    rm -f *.o x11_Drawing
