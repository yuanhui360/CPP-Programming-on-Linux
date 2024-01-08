CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcrypto -lssl

all: evp_display evp_file_encrypt evp_file_decrypt

evp_display: evp_display.o evp_class.o
	$(CC) $(LDFLAGS) evp_display.o evp_class.o -o evp_display
evp_display.o: evp_display.cpp
	$(CC) $(CFLAGS) evp_display.cpp -o evp_display.o

evp_file_encrypt: evp_file_encrypt.o evp_class.o
	$(CC) $(LDFLAGS) evp_file_encrypt.o evp_class.o -o evp_file_encrypt
evp_file_encrypt.o: evp_file_encrypt.cpp
	$(CC) $(CFLAGS) evp_file_encrypt.cpp -o evp_file_encrypt.o

evp_file_decrypt: evp_file_decrypt.o evp_class.o
	$(CC) $(LDFLAGS) evp_file_decrypt.o evp_class.o -o evp_file_decrypt
evp_file_decrypt.o: evp_file_decrypt.cpp
	$(CC) $(CFLAGS) evp_file_decrypt.cpp -o evp_file_decrypt.o

evp_class.o: evp_class.cpp
	$(CC) $(CFLAGS) evp_class.cpp -o evp_class.o

clean:
	rm -f *.o evp_display evp_file_encrypt evp_file_decrypt
