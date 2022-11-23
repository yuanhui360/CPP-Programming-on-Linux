
CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcryptopp -lcrypto -lssl -lrt -lpthread -lboost_system

all: gzip_compress gzip_extract

gzip_compress: gzip_compress.o
    $(CC) $(LDFLAGS) gzip_compress.o -o gzip_compress
gzip_compress.o: gzip_compress.cpp
    $(CC) $(CFLAGS) gzip_compress.cpp -o gzip_compress.o

gzip_extract: gzip_extract.o
    $(CC) $(LDFLAGS) gzip_extract.o -o gzip_extract
gzip_extract.o: gzip_extract.cpp
    $(CC) $(CFLAGS) gzip_extract.cpp -o gzip_extract.o

clean:
    rm -f *.o gzip_compress gzip_extract
