#
# Hui test g++ Makefile on Linux
# Note : It may require to replace some front space characters with <tab> after you downloaded this Makefile
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lssh -lpthread

all: ssh_cmd_blocking ssh_cpp_remote_shell ssh_cmd_remote_shell

ssh_cmd_blocking: ssh_cmd_blocking.o
    $(CC) ssh_cmd_blocking.o -o ssh_cmd_blocking $(LDFLAGS)
ssh_cmd_blocking.o: ssh_cmd_blocking.cpp
    $(CC) $(CFLAGS) ssh_cmd_blocking.cpp $(LDFLAGS)

ssh_cmd_remote_shell: ssh_cmd_remote_shell.o
    $(CC) ssh_cmd_remote_shell.o -o ssh_cmd_remote_shell $(LDFLAGS)
ssh_cmd_remote_shell.o: ssh_cmd_remote_shell.cpp
    $(CC) $(CFLAGS) ssh_cmd_remote_shell.cpp $(LDFLAGS)

ssh_cpp_remote_shell: ssh_cpp_remote_shell.o
    $(CC) ssh_cpp_remote_shell.o -o ssh_cpp_remote_shell $(LDFLAGS)
ssh_cpp_remote_shell.o: ssh_cpp_remote_shell.cpp
    $(CC) $(CFLAGS) ssh_cpp_remote_shell.cpp $(LDFLAGS)

clean:
    rm -f *.o ssh_cmd_blocking ssh_cpp_remote_shell ssh_cmd_remote_shell
