CC=g++
CFLAGS=-c -Wall -Werror -g -I/usr/include/mongocxx -I/usr/include/bsoncxx
LDFLAGS = -lm  -lrestbed -L/usr/lib/x86_64-linux-gnu -lmongocxx -lbsoncxx --std=c++17


all: myWebServiceGet myWebServiceGetParam

myWebServiceGet: myWebServiceGet.o
    $(CC) $(LDFLAGS) myWebServiceGet.o -o myWebServiceGet
myWebServiceGet.o: myWebServiceGet.cpp
    $(CC) $(CFLAGS) myWebServiceGet.cpp

myWebServiceGetParam: myWebServiceGetParam.o
    $(CC) $(LDFLAGS) myWebServiceGetParam.o -o myWebServiceGetParam
myWebServiceGetParam.o: myWebServiceGetParam.cpp
    $(CC) $(CFLAGS) myWebServiceGetParam.cpp

clean:
    rm -f *.o myWebServiceGet myWebServiceGetParam
