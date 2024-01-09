CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcrypto -lssl -lrt -lpthread -lboost_system

all: evp_generate_rsakey pkey_sign pkey_verify

pkey_verify: pkey_verify.o evp_class.o
	$(CC) pkey_verify.o evp_class.o -o pkey_verify $(LDFLAGS)
pkey_verify.o: pkey_verify.cpp
	$(CC) $(CFLAGS) pkey_verify.cpp -o pkey_verify.o

pkey_sign: pkey_sign.o evp_class.o
	$(CC) pkey_sign.o evp_class.o -o pkey_sign $(LDFLAGS)
pkey_sign.o: pkey_sign.cpp
	$(CC) $(CFLAGS) pkey_sign.cpp -o pkey_sign.o

evp_generate_rsakey: evp_generate_rsakey.o evp_class.o
	$(CC) evp_generate_rsakey.o evp_class.o -o evp_generate_rsakey $(LDFLAGS)
evp_generate_rsakey.o: evp_generate_rsakey.cpp
	$(CC) $(CFLAGS) evp_generate_rsakey.cpp -o evp_generate_rsakey.o

evp_class.o: evp_class.cpp
	$(CC) $(CFLAGS) evp_class.cpp -o evp_class.o

clean:
	rm -f *.o evp_generate_rsakey pkey_sign pkey_verify
