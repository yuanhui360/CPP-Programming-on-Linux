#
# Hui test g++ Makefile on Linux
# Note: It might require to replace the beginning space character with <TAB> after download the Makefile
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lX11 -lssh -lpthread

all: x11_Drawing x11_display_xbm

x11_Drawing: x11_Drawing.o
    $(CC) x11_Drawing.o -o x11_Drawing $(LDFLAGS)
x11_Drawing.o: x11_Drawing.cpp
    $(CC) $(CFLAGS) x11_Drawing.cpp $(LDFLAGS)

x11_display_xbm: x11_display_xbm.o
    $(CC) x11_display_xbm.o -o x11_display_xbm $(LDFLAGS)
x11_display_xbm.o: x11_display_xbm.cpp
    $(CC) $(CFLAGS) x11_display_xbm.cpp $(LDFLAGS)

clean:
    rm -f *.o x11_Drawing x11_display_xbm
