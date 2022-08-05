#
# Hui test g++ Makefile on Linux
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcryptopp -lcrypto -lssl -lrt -lpthread -lboost_system

all: file_copy file_read base64_file_enc base64_file_dec base64_mgs_enc

file_read: file_read.o
    $(CC) $(LDFLAGS) file_read.o -o file_read
file_read.o: file_read.cpp
    $(CC) $(CFLAGS) file_read.cpp -o file_read.o

file_copy: file_copy.o
    $(CC) $(LDFLAGS) file_copy.o -o file_copy
file_copy.o: file_copy.cpp
    $(CC) $(CFLAGS) file_copy.cpp -o file_copy.o

base64_file_enc: base64_file_enc.o
    $(CC) $(LDFLAGS) base64_file_enc.o -o base64_file_enc
base64_file_enc.o: base64_file_enc.cpp
    $(CC) $(CFLAGS) base64_file_enc.cpp -o base64_file_enc.o

base64_file_dec: base64_file_dec.o
    $(CC) $(LDFLAGS) base64_file_dec.o -o base64_file_dec
base64_file_dec.o: base64_file_dec.cpp
    $(CC) $(CFLAGS) base64_file_dec.cpp -o base64_file_dec.o

base64_mgs_enc: base64_mgs_enc.o
    $(CC) $(LDFLAGS) base64_mgs_enc.o -o base64_mgs_enc
base64_mgs_enc.o: base64_mgs_enc.cpp
    $(CC) $(CFLAGS) base64_mgs_enc.cpp -o base64_mgs_enc.o

clean:
    rm -f *.o file_copy file_read base64_file_enc base64_file_dec base64_mgs_enc
