#
# Hui test g++ Makefile on Linux
# Please Note : Once downloaded this Makefile, it may require replace front space characters to <TAB>

CC=g++
CFLAGS=-c -Wall
LDFLAGS = -levent -levent_core -levent_extra -lcrypto -lssl -lwolfssl

all: wolfssl_rsa_keygen wolfssl_rsa_certgen

wolfssl_rsa_keygen: wolfssl_rsa_keygen.o
    $(CC) wolfssl_rsa_keygen.o $(LDFLAGS) -o wolfssl_rsa_keygen
wolfssl_rsa_keygen.o: wolfssl_rsa_keygen.cpp
    $(CC) $(CFLAGS) wolfssl_rsa_keygen.cpp -o  wolfssl_rsa_keygen.o

wolfssl_rsa_certgen: wolfssl_rsa_certgen.o
    $(CC) wolfssl_rsa_certgen.o $(LDFLAGS) -o wolfssl_rsa_certgen
wolfssl_rsa_certgen.o: wolfssl_rsa_certgen.cpp
    $(CC) $(CFLAGS) wolfssl_rsa_certgen.cpp -o  wolfssl_rsa_certgen.o

clean:
    rm -f *.o wolfssl_rsa_keygen wolfssl_rsa_certgen
