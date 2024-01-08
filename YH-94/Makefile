CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lboost_json -lboost_system -std=c++11

OBJ_LIST = json_parse.o MyParserHandler.o
all: json_parse

json_parse: ${OBJ_LIST}
	$(CC) ${OBJ_LIST} -o json_parse $(LDFLAGS)
json_parse.o: json_parse.cpp
	$(CC) $(CFLAGS) json_parse.cpp -o json_parse.o
MyParserHandler.o: MyParserHandler.cpp
	$(CC) $(CFLAGS) MyParserHandler.cpp -o MyParserHandler.o

clean:
	rm -f *.o json_parse
