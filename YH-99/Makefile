CC=g++
CFLAGS=-c -Wall -I/usr/local/include/google/protobuf
LDFLAGS =  -L/usr/local/lib -lprotobuf

all: coded_output coded_input

coded_output: coded_output.o
	$(CC) coded_output.o -o coded_output $(LDFLAGS)
coded_output.o: coded_output.cpp
	$(CC) $(CFLAGS) coded_output.cpp -o coded_output.o

coded_input: coded_input.o
	$(CC) coded_input.o -o coded_input $(LDFLAGS)
coded_input.o: coded_input.cpp
	$(CC) $(CFLAGS) coded_input.cpp -o coded_input.o

clean:
	rm -f *.o coded_output coded_input
