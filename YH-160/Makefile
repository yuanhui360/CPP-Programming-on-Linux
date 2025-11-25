
# Hui test g++ Makefile on Linux
# Note : It might require to replace front spaces characters with <TAB> 

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lssh -lpthread

all: ssh_data_client ssh_echo_server ssh_rsa_keygen

ssh_data_client: ssh_data_client.o
    $(CC) ssh_data_client.o -o ssh_data_client $(LDFLAGS)
ssh_data_client.o: ssh_data_client.cpp
    $(CC) $(CFLAGS) ssh_data_client.cpp $(LDFLAGS)

ssh_echo_server: ssh_echo_server.o
    $(CC) ssh_echo_server.o -o ssh_echo_server $(LDFLAGS)
ssh_echo_server.o: ssh_echo_server.cpp
    $(CC) $(CFLAGS) ssh_echo_server.cpp $(LDFLAGS)

ssh_rsa_keygen: ssh_rsa_keygen.o
    $(CC) ssh_rsa_keygen.o -o ssh_rsa_keygen $(LDFLAGS)
ssh_rsa_keygen.o: ssh_rsa_keygen.cpp
    $(CC) $(CFLAGS) ssh_rsa_keygen.cpp $(LDFLAGS)

clean:
    rm -f *.o ssh_data_client ssh_echo_server ssh_rsa_keygen
