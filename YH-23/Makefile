CC=g++
CFLAGS=-c -Wall -Werror -g -I/usr/locaal/include/mongocxx -I/usr/local/include/bsoncxx
LDFLAGS = -L/usr/local/lib -lmongocxx -lbsoncxx --std=c++17

all: mongo_connect mongo_find_all

mongo_connect: mongo_connect.o
    $(CC) mongo_connect.o -o mongo_connect $(LDFLAGS)
mongo_connect.o: mongo_connect.cpp
    $(CC) $(CFLAGS) mongo_connect.cpp $(LDFLAGS)

mongo_find_all: mongo_find_all.o
    $(CC) mongo_find_all.o -o mongo_find_all $(LDFLAGS)
mongo_find_all.o: mongo_find_all.cpp
    $(CC) $(CFLAGS) mongo_find_all.cpp $(LDFLAGS)

clean:
    rm -f *.o mongo_connect mongo_find_all
