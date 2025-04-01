#
# Hui test g++ Makefile on Linux
# After you downloaded this file, it might need to replace front space to <TAB>

CC=g++
CFLAGS=-c -Wall
LDFLAGS = -lusb-1.0


all: libusb_list_dev libusb_hotplug

libusb_list_dev: libusb_list_dev.o libusb_desc.o
    $(CC) libusb_desc.o libusb_list_dev.o $(LDFLAGS) -o libusb_list_dev
libusb_list_dev.o: libusb_list_dev.cpp
    $(CC) $(CFLAGS) libusb_list_dev.cpp -o  libusb_list_dev.o

libusb_hotplug: libusb_hotplug.o libusb_desc.o
    $(CC) libusb_desc.o libusb_hotplug.o $(LDFLAGS) -o libusb_hotplug
libusb_hotplug.o: libusb_hotplug.cpp
    $(CC) $(CFLAGS) libusb_hotplug.cpp -o  libusb_hotplug.o

libusb_desc.o: libusb_desc.cpp
    $(CC) $(CFLAGS) libusb_desc.cpp -o libusb_desc.o

clean:
    rm -f *.o libusb_list_dev libusb_hotplug
