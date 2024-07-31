#
# Hui test g++ Makefile on Linux
#

CC=g++
CFLAGS=-c -Wall -Werror -g -I/usr/local/include/mongocxx -I/usr/local/include/bsoncxx
LDFLAGS = -L/usr/local/lib -lmongocxx -lbsoncxx -lssh --std=c++17

all: mongo_connect mongo_query mongo_atlas_connect mongo_tls_connect

mongo_connect: mongo_connect.o
  $(CC) mongo_connect.o -o mongo_connect $(LDFLAGS)
mongo_connect.o: mongo_connect.cpp
  $(CC) $(CFLAGS) mongo_connect.cpp $(LDFLAGS)

mongo_query: mongo_query.o
  $(CC) mongo_query.o -o mongo_query $(LDFLAGS)
mongo_query.o: mongo_query.cpp
  $(CC) $(CFLAGS) mongo_query.cpp $(LDFLAGS)

mongo_atlas_connect: mongo_atlas_connect.o
  $(CC) mongo_atlas_connect.o -o mongo_atlas_connect $(LDFLAGS)
mongo_atlas_connect.o: mongo_atlas_connect.cpp
  $(CC) $(CFLAGS) mongo_atlas_connect.cpp $(LDFLAGS)

mongo_tls_connect: mongo_tls_connect.o
  $(CC) mongo_tls_connect.o -o mongo_tls_connect $(LDFLAGS)
mongo_tls_connect.o: mongo_tls_connect.cpp
  $(CC) $(CFLAGS) mongo_tls_connect.cpp $(LDFLAGS)

clean:
  rm -f *.o mongo_connect mongo_query mongo_atlas_connect mongo_tls_connect
