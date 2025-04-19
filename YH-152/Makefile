#
# Hui test g++ Makefile on Linux
# It might need replace front space characters with <TAB> after you downloaded this makefile
#

CC=g++
CFLAGS=-c -Wall
LDFLAGS = -lusb-1.0


all: libusb_hotplug

libusb_hotplug: libusb_hotplug.o libusb_desc.o
    $(CC) libusb_desc.o libusb_hotplug.o $(LDFLAGS) -o libusb_hotplug
libusb_hotplug.o: libusb_hotplug.cpp
    $(CC) $(CFLAGS) libusb_hotplug.cpp -o  libusb_hotplug.o

libusb_desc.o: libusb_desc.cpp
    $(CC) $(CFLAGS) libusb_desc.cpp -o libusb_desc.o

clean:
    rm -f *.o libusb_hotplug
