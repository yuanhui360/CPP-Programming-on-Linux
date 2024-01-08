CC=g++
CFLAGS=-c -Wall -Werror -g -I/usr/include/mongocxx -I/usr/include/bsoncxx
LDFLAGS = -lcurl -lboost_json -lpthread -lboost_system  -lrestbed -lmongocxx -lbsoncxx --std=c++17

all: EchoGetPostWS curl_http_get curl_http_post

EchoGetPostWS: EchoGetPostWS.o
	$(CC) $(LDFLAGS) EchoGetPostWS.o -o EchoGetPostWS
EchoGetPostWS.o: EchoGetPostWS.cpp
	$(CC) $(CFLAGS) EchoGetPostWS.cpp

curl_http_get: curl_http_get.o
	$(CC) $(LDFLAGS) curl_http_get.o -o curl_http_get
curl_http_get.o: curl_http_get.cpp
	$(CC) $(CFLAGS) curl_http_get.cpp -o curl_http_get.o

curl_http_post: curl_http_post.o
	$(CC) $(LDFLAGS) curl_http_post.o -o curl_http_post
curl_http_post.o: curl_http_post.cpp
	$(CC) $(CFLAGS) curl_http_post.cpp -o curl_http_post.o

clean:
	rm -f *.o EchoGetPostWS curl_http_get curl_http_post
