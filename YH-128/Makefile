#
# Hui test g++ Makefile on Linux
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcryptopp -lcrypto -lssl -lrt -lpthread -lboost_system

all: rsakey_load rsakey_gen rsakey_encrypt rsakey_decrypt rsakey_encrypt_large rsakey_decrypt_large

rsakey_encrypt: rsakey_encrypt.o
	$(CC) $(LDFLAGS) rsakey_encrypt.o -o rsakey_encrypt
rsakey_encrypt.o: rsakey_encrypt.cpp
	$(CC) $(CFLAGS) rsakey_encrypt.cpp -o rsakey_encrypt.o

rsakey_decrypt: rsakey_decrypt.o
	$(CC) $(LDFLAGS) rsakey_decrypt.o -o rsakey_decrypt
rsakey_decrypt.o: rsakey_decrypt.cpp
	$(CC) $(CFLAGS) rsakey_decrypt.cpp -o rsakey_decrypt.o

rsakey_encrypt_large: rsakey_encrypt_large.o
	$(CC) $(LDFLAGS) rsakey_encrypt_large.o -o rsakey_encrypt_large
rsakey_encrypt_large.o: rsakey_encrypt_large.cpp
	$(CC) $(CFLAGS) rsakey_encrypt_large.cpp -o rsakey_encrypt_large.o

rsakey_decrypt_large: rsakey_decrypt_large.o
	$(CC) $(LDFLAGS) rsakey_decrypt_large.o -o rsakey_decrypt_large
rsakey_decrypt_large.o: rsakey_decrypt_large.cpp
	$(CC) $(CFLAGS) rsakey_decrypt_large.cpp -o rsakey_decrypt_large.o

rsakey_gen: rsakey_gen.o
	$(CC) $(LDFLAGS) rsakey_gen.o -o rsakey_gen
rsakey_gen.o: rsakey_gen.cpp
	$(CC) $(CFLAGS) rsakey_gen.cpp -o rsakey_gen.o

rsakey_load: rsakey_load.o
	$(CC) $(LDFLAGS) rsakey_load.o -o rsakey_load
rsakey_load.o: rsakey_load.cpp
	$(CC) $(CFLAGS) rsakey_load.cpp -o rsakey_load.o

clean:
	rm -f *.o rsakey_load rsakey_gen rsakey_encrypt rsakey_decrypt rsakey_encrypt_large rsakey_decrypt_large
