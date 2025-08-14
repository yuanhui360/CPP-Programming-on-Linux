
# Hui test g++ Makefile on Linux
# Note: it might need to replace front space characters with <TAB> after downloaded

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lssh -lpthread

all: ssh_cmd ssh_cmd_blocking ssh_cmd_non_blocking

ssh_cmd: ssh_cmd.o
    $(CC) ssh_cmd.o -o ssh_cmd $(LDFLAGS)
ssh_cmd.o: ssh_cmd.cpp
    $(CC) $(CFLAGS) ssh_cmd.cpp $(LDFLAGS)

ssh_cmd_blocking: ssh_cmd_blocking.o
    $(CC) ssh_cmd_blocking.o -o ssh_cmd_blocking $(LDFLAGS)
ssh_cmd_blocking.o: ssh_cmd_blocking.cpp
    $(CC) $(CFLAGS) ssh_cmd_blocking.cpp $(LDFLAGS)

ssh_cmd_non_blocking: ssh_cmd_non_blocking.o
    $(CC) ssh_cmd_non_blocking.o -o ssh_cmd_non_blocking $(LDFLAGS)
ssh_cmd_non_blocking.o: ssh_cmd_non_blocking.cpp
    $(CC) $(CFLAGS) ssh_cmd_non_blocking.cpp $(LDFLAGS)

clean:
    rm -f *.o ssh_cmd ssh_cmd_blocking ssh_cmd_non_blocking
