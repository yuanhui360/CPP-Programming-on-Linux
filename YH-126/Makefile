CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcryptopp -lcrypto -lssl -lrt -lpthread -lboost_system

all: aes_encrypt aes_decrypt

aes_encrypt: aes_encrypt.o
    $(CC) $(LDFLAGS) aes_encrypt.o -o aes_encrypt
aes_encrypt.o: aes_encrypt.cpp
    $(CC) $(CFLAGS) aes_encrypt.cpp -o aes_encrypt.o

aes_decrypt: aes_decrypt.o
    $(CC) $(LDFLAGS) aes_decrypt.o -o aes_decrypt
aes_decrypt.o: aes_decrypt.cpp
    $(CC) $(CFLAGS) aes_decrypt.cpp -o aes_decrypt.o

clean:
    rm -f *.o aes_encrypt aes_decrypt
