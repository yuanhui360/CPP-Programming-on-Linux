#
# Hui test g++ Makefile on Linux
# Note:
# After fownloaded this Makefile, it might need to replace front space characters with <TAB>

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lssh -lpthread

all: ssh_cmd ssh_cpp_non_exception ssh_cpp_exception

ssh_cmd: ssh_cmd.o
    $(CC) ssh_cmd.o -o ssh_cmd $(LDFLAGS)
ssh_cmd.o: ssh_cmd.cpp
    $(CC) $(CFLAGS) ssh_cmd.cpp $(LDFLAGS)

ssh_cpp_exception: ssh_cpp_exception.o
    $(CC) ssh_cpp_exception.o -o ssh_cpp_exception $(LDFLAGS)
ssh_cpp_exception.o: ssh_cpp_exception.cpp
    $(CC) $(CFLAGS) ssh_cpp_exception.cpp $(LDFLAGS)

ssh_cpp_non_exception: ssh_cpp_non_exception.o
    $(CC) ssh_cpp_non_exception.o -o ssh_cpp_non_exception $(LDFLAGS)
ssh_cpp_non_exception.o: ssh_cpp_non_exception.cpp
    $(CC) $(CFLAGS) ssh_cpp_non_exception.cpp $(LDFLAGS)

clean:
    rm -f *.o ssh_cmd ssh_cpp_non_exception ssh_cpp_exception
