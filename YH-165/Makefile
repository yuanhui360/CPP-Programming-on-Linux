
# Hui test g++ Makefile on Linux
# Note: it may require to replace front space characters with <TAB> after you downlaoded this Makefile

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lX11 -lstb -lxcb

all: libx11_stb_image libxcb_stb_image

libx11_stb_image: libx11_stb_image.o
    $(CC) libx11_stb_image.o -o libx11_stb_image $(LDFLAGS)
libx11_stb_image.o: libx11_stb_image.cpp
    $(CC) $(CFLAGS) libx11_stb_image.cpp $(LDFLAGS)

libxcb_stb_image: libxcb_stb_image.o
    $(CC) libxcb_stb_image.o -o libxcb_stb_image $(LDFLAGS)
libxcb_stb_image.o: libxcb_stb_image.cpp
    $(CC) $(CFLAGS) libxcb_stb_image.cpp $(LDFLAGS)

clean:
    rm -f *.o libx11_stb_image libxcb_stb_image
