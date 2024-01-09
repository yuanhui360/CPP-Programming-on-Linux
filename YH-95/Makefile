CC=g++
CFLAGS=-c -Wall -fPIC -I/usr/include/python2.7  -I/usr/local/include/boost
LDFLAGS =  -shared -Wl,--export-dynamic -lboost_python27 -L/usr/lib/python2.7/config-x86_64-linux-gnu -lpython2.7

all: MyPyLib.so

MyPyLib.so: MyFunc.o
	$(CC) MyFunc.o -o MyPyLib.so $(LDFLAGS)
MyFunc.o: MyFunc.cpp
	$(CC) $(CFLAGS) MyFunc.cpp -o MyFunc.o

clean:
	rm -f *.o MyPyLib.so
