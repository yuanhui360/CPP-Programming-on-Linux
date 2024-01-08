CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcryptopp -lcrypto -lssl -lrt -lpthread -lboost_system

all: md5_size md5_digest sha256_digest

md5_size: md5_size.o
	$(CC) md5_size.o -o md5_size $(LDFLAGS)
md5_size.o: md5_size.cpp
	$(CC) $(CFLAGS) md5_size.cpp -o md5_size.o

md5_digest: md5_digest.o
	$(CC) md5_digest.o -o md5_digest $(LDFLAGS)
md5_digest.o: md5_digest.cpp
	$(CC) $(CFLAGS) md5_digest.cpp -o md5_digest.o

sha256_digest: sha256_digest.o
	$(CC) sha256_digest.o -o sha256_digest $(LDFLAGS)
sha256_digest.o: sha256_digest.cpp
	$(CC) $(CFLAGS) sha256_digest.cpp -o sha256_digest.o

clean:
	rm -f *.o md5_size md5_digest sha256_digest
