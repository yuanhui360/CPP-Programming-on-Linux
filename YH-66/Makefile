CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -laws-cpp-sdk-core -laws-cpp-sdk-dynamodb -laws-cpp-sdk-dynamodbstreams

all: get_item scan_table desc_table list_table load_json_file update_item query_item

get_item: get_item.o
	$(CC) get_item.o -o get_item $(LDFLAGS)
get_item.o: get_item.cpp
	$(CC) $(CFLAGS) get_item.cpp

query_item: query_item.o
	$(CC) query_item.o -o query_item $(LDFLAGS)
query_item.o: query_item.cpp
	$(CC) $(CFLAGS) query_item.cpp

scan_table: scan_table.o
	$(CC) scan_table.o -o scan_table $(LDFLAGS)
scan_table.o: scan_table.cpp
	$(CC) $(CFLAGS) scan_table.cpp

desc_table: desc_table.o
	$(CC) desc_table.o -o desc_table $(LDFLAGS)
desc_table.o: desc_table.cpp
	$(CC) $(CFLAGS) desc_table.cpp

list_table: list_table.o
	$(CC) list_table.o -o list_table $(LDFLAGS)
list_table.o: list_table.cpp
	$(CC) $(CFLAGS) list_table.cpp

load_json_file: load_json_file.o
	$(CC) load_json_file.o -o load_json_file $(LDFLAGS)
load_json_file.o: load_json_file.cpp
	$(CC) $(CFLAGS) load_json_file.cpp

update_item: update_item.o
	$(CC) update_item.o -o update_item $(LDFLAGS)
update_item.o: update_item.cpp
	$(CC) $(CFLAGS) update_item.cpp

clean:
	rm -f *.o get_item scan_table desc_table list_table load_json_file update_item query_item
