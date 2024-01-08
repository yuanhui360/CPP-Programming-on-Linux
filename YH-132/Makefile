CC=g++
CFLAGS=-c -Wall -Werror -g -I/usr/include/mongocxx -I/usr/include/bsoncxx
LDFLAGS = -lcurl -lboost_json -lpthread -lboost_system  -lrestbed -lmongocxx -lbsoncxx --std=c++17

all: EchoGetPostWS curl_https_get curl_https_post

EchoGetPostWS: EchoGetPostWS.o
	$(CC) $(LDFLAGS) EchoGetPostWS.o -o EchoGetPostWS
EchoGetPostWS.o: EchoGetPostWS.cpp
	$(CC) $(CFLAGS) EchoGetPostWS.cpp

curl_https_get: curl_https_get.o
	$(CC) $(LDFLAGS) curl_https_get.o -o curl_https_get
curl_https_get.o: curl_https_get.cpp
	$(CC) $(CFLAGS) curl_https_get.cpp -o curl_https_get.o

curl_https_post: curl_https_post.o
	$(CC) $(LDFLAGS) curl_https_post.o -o curl_https_post
curl_https_post.o: curl_https_post.cpp
	$(CC) $(CFLAGS) curl_https_post.cpp -o curl_https_post.o

clean:
	rm -f *.o EchoGetPostWS curl_https_get curl_https_post
