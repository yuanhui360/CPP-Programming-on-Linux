CC=g++
CFLAGS=-c -Wall -Werror -g -I/usr/include/mongocxx -I/usr/include/bsoncxx
LDFLAGS = -lm  -lrestbed -L/usr/lib/x86_64-linux-gnu -lmongocxx -lbsoncxx --std=c++17


all: myWebServiceGet myWebServiceGetParam

myWebServiceGet: myWebServiceGet.o
	$(CC) myWebServiceGet.o -o myWebServiceGet $(LDFLAGS)
myWebServiceGet.o: myWebServiceGet.cpp
	$(CC) $(CFLAGS) myWebServiceGet.cpp

myWebServiceGetParam: myWebServiceGetParam.o
	$(CC) myWebServiceGetParam.o -o myWebServiceGetParam $(LDFLAGS)
myWebServiceGetParam.o: myWebServiceGetParam.cpp
	$(CC) $(CFLAGS) myWebServiceGetParam.cpp

clean:
	rm -f *.o myWebServiceGet myWebServiceGetParam
