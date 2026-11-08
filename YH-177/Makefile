#
# Hui test g++ Makefile on Linux
# Note : it may require to replace front space characters with <TAB> after you downloaded this makefile
#

CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -std=c++17 -lc -lpthread -lev

all: libev_ev_fork_exec my_basic_fork_exec

libev_ev_fork_exec: libev_ev_fork_exec.o
        $(CC) libev_ev_fork_exec.o $(LDFLAGS) -o libev_ev_fork_exec
libev_ev_fork_exec.o: libev_ev_fork_exec.cpp
        $(CC) $(CFLAGS) libev_ev_fork_exec.cpp $(LDFLAGS) -o libev_ev_fork_exec.o

my_basic_fork_exec: my_basic_fork_exec.o
        $(CC) my_basic_fork_exec.o $(LDFLAGS) -o my_basic_fork_exec
my_basic_fork_exec.o: my_basic_fork_exec.cpp
        $(CC) $(CFLAGS) my_basic_fork_exec.cpp $(LDFLAGS) -o my_basic_fork_exec.o

clean:
        rm -f *.o libev_ev_fork_exec my_basic_fork_exec
