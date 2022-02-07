CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcrypto -lssl

all: evp_readkey_file evp_generate_rsakey

evp_generate_rsakey: evp_generate_rsakey.o evp_class.o
    $(CC) $(LDFLAGS) evp_generate_rsakey.o evp_class.o -o evp_generate_rsakey
evp_generate_rsakey.o: evp_generate_rsakey.cpp
    $(CC) $(CFLAGS) evp_generate_rsakey.cpp -o evp_generate_rsakey.o

evp_readkey_file: evp_readkey_file.o evp_class.o
    $(CC) $(LDFLAGS) evp_readkey_file.o evp_class.o -o evp_readkey_file
evp_readkey_file.o: evp_readkey_file.cpp
    $(CC) $(CFLAGS) evp_readkey_file.cpp -o evp_readkey_file.o

evp_class.o: evp_class.cpp
    $(CC) $(CFLAGS) evp_class.cpp -o evp_class.o

clean:
    rm -f *.o evp_readkey_file evp_generate_rsakey
