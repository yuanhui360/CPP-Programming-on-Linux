CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lboost_json -lboost_system -std=c++11

all: json_valid

json_valid: json_valid.o
	$(CC) json_valid.o  -o json_valid $(LDFLAGS)
json_valid.o: json_valid.cpp
	$(CC) $(CFLAGS) json_valid.cpp -o json_valid.o

clean:
	rm -f *.o json_valid
