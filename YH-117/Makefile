CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcrypto -lssl

all: bio_file bio_b64_encode bio_b64_decoded

bio_file: bio_file.o
	$(CC) bio_file.o -o bio_file $(LDFLAGS)
bio_file.o: bio_file.cpp
	$(CC) $(CFLAGS) bio_file.cpp -o bio_file.o

bio_b64_encode: bio_b64_encode.o
	$(CC) bio_b64_encode.o -o bio_b64_encode $(LDFLAGS)
bio_b64_encode.o: bio_b64_encode.cpp
	$(CC) $(CFLAGS) bio_b64_encode.cpp -o bio_b64_encode.o

bio_b64_decoded: bio_b64_decoded.o
	$(CC) bio_b64_decoded.o -o bio_b64_decoded $(LDFLAGS)
bio_b64_decoded.o: bio_b64_decoded.cpp
	$(CC) $(CFLAGS) bio_b64_decoded.cpp -o bio_b64_decoded.o

clean:
	rm -f *.o bio_file bio_b64_encode bio_b64_decoded
