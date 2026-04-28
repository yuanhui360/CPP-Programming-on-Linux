#
# Hui test g++ Makefile on Linux
#    Note: It may require to replace front space characters with <TAB> after downloaded this Makefile
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lxcb -lX11 -lssh -lpthread

all: libxcb_drawing

libxcb_drawing: libxcb_drawing.o
    $(CC) libxcb_drawing.o -o libxcb_drawing $(LDFLAGS)
libxcb_drawing.o: libxcb_drawing.cpp
    $(CC) $(CFLAGS) libxcb_drawing.cpp $(LDFLAGS)

clean:
    rm -f *.o libxcb_drawing
