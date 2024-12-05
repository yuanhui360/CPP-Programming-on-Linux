#
# Hui test g++ Makefile on Linux
# It may require to adjust all leading space with <tab> after you downloaded Makefile
#

CC=g++
CFLAGS=-c -Wall
LDFLAGS = -levent -levent_core -levent_extra -lcrypto -lssl -lwolfssl

all: wolfssl_aescbc_decrypt wolfssl_aescbc_encrypt

wolfssl_aescbc_decrypt: wolfssl_aescbc_decrypt.o
    $(CC) wolfssl_aescbc_decrypt.o $(LDFLAGS) -o wolfssl_aescbc_decrypt
wolfssl_aescbc_decrypt.o: wolfssl_aescbc_decrypt.cpp
    $(CC) $(CFLAGS) wolfssl_aescbc_decrypt.cpp -o  wolfssl_aescbc_decrypt.o

wolfssl_aescbc_encrypt: wolfssl_aescbc_encrypt.o
    $(CC) wolfssl_aescbc_encrypt.o $(LDFLAGS) -o wolfssl_aescbc_encrypt
wolfssl_aescbc_encrypt.o: wolfssl_aescbc_encrypt.cpp
    $(CC) $(CFLAGS) wolfssl_aescbc_encrypt.cpp -o  wolfssl_aescbc_encrypt.o

clean:
    rm -f *.o wolfssl_aescbc_decrypt wolfssl_aescbc_encrypt
