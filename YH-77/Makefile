CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lpthread

all: rlimit_ex rusage_ex

rlimit_ex: rlimit_ex.o
    $(CC) rlimit_ex.o -o rlimit_ex
rlimit_ex.o: rlimit_ex.cpp
    $(CC) $(CFLAGS) rlimit_ex.cpp $(LDFLAGS)

rusage_ex: rusage_ex.o
    $(CC) $(LDFLAGS) rusage_ex.o -o rusage_ex
rusage_ex.o: rusage_ex.cpp
    $(CC) $(CFLAGS) rusage_ex.cpp

clean:
    rm -f *.o rlimit_ex rusage_ex
