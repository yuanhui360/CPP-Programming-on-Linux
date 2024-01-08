
CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcryptopp -lcrypto -lssl -lrt -lpthread -lboost_system

all: rsass_sign rsass_verify

rsass_sign: rsass_sign.o
	$(CC) $(LDFLAGS) rsass_sign.o -o rsass_sign
rsass_sign.o: rsass_sign.cpp
	$(CC) $(CFLAGS) rsass_sign.cpp -o rsass_sign.o

rsass_verify: rsass_verify.o
	$(CC) $(LDFLAGS) rsass_verify.o -o rsass_verify
rsass_verify.o: rsass_verify.cpp
	$(CC) $(CFLAGS) rsass_verify.cpp -o rsass_verify.o

clean:
	rm -f *.o rsass_sign rsass_verify
