CC=g++
CFLAGS=-c -Wall -I/usr/local/include/google/protobuf
LDFLAGS =  -L/usr/local/lib -lprotobuf

all: writePO readPO

writePO: writePO.o Order.pb.o
    $(CC) $(LDFLAGS) writePO.o Order.pb.o -o writePO
writePO.o: writePO.cpp
    $(CC) $(CFLAGS) writePO.cpp -o writePO.o

readPO: readPO.o Order.pb.o
    $(CC) $(LDFLAGS) readPO.o Order.pb.o -o readPO
readPO.o: readPO.cpp
    $(CC) $(CFLAGS) readPO.cpp -o readPO.o

Order.pb.o: Order.pb.cc
    $(CC) $(CFLAGS) Order.pb.cc -o Order.pb.o

clean:
    rm -f *.o writePO readPO
