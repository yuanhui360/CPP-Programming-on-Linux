CC=g++
CFLAGS=-c -Wall  -I/usr/include/mysql-cppconn-8  -I../include
LDFLAGS = -lm  -L/usr/lib/x86_64-linux-gnu  -lmysqlcppconn


all: mySqlSelect mySqlConn

mySqlSelect: mySqlSelect.o
    $(CC) $(LDFLAGS) mySqlSelect.o -o mySqlSelect
mySqlSelect.o: mySqlSelect.cpp
    $(CC) $(CFLAGS) mySqlSelect.cpp -o  mySqlSelect.o

mySqlConn: mySqlConn.o
    $(CC) $(LDFLAGS) mySqlConn.o -o mySqlConn
mySqlConn.o: mySqlConn.cpp
    $(CC) $(CFLAGS) mySqlConn.cpp -o  mySqlConn.o
    
clean:
    rm -f *.o mySqlSelect mySqlConn
