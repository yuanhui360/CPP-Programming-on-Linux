CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcrypto -lssl

all: evp_digest_disp evp_digest_file evp_digest_verify

evp_digest_disp: evp_digest_disp.o
	$(CC) $(LDFLAGS) evp_digest_disp.o -o evp_digest_disp
evp_digest_disp.o: evp_digest_disp.cpp
	$(CC) $(CFLAGS) evp_digest_disp.cpp -o evp_digest_disp.o

evp_digest_file: evp_digest_file.o
	$(CC) $(LDFLAGS) evp_digest_file.o -o evp_digest_file
evp_digest_file.o: evp_digest_file.cpp
	$(CC) $(CFLAGS) evp_digest_file.cpp -o evp_digest_file.o

evp_digest_verify: evp_digest_verify.o
	$(CC) $(LDFLAGS) evp_digest_verify.o -o evp_digest_verify
evp_digest_verify.o: evp_digest_verify.cpp
	$(CC) $(CFLAGS) evp_digest_verify.cpp -o evp_digest_verify.o

clean:
	rm -f *.o evp_digest_disp evp_digest_file evp_digest_verify
