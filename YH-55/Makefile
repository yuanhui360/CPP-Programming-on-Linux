CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lssh

all: ssh_client

ssh_client: ssh_client.o
	$(CC) ssh_client.o -o ssh_client $(LDFLAGS)
ssh_client.o: ssh_client.cpp
	$(CC) $(CFLAGS) ssh_client.cpp $(LDFLAGS)

clean:
	rm -f *.o ssh_client
