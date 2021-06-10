CC=g++
CFLAGS=-c -Wall  -I/usr/include/mysql-cppconn-8  -I../include
LDFLAGS = -lm  -L/usr/lib/x86_64-linux-gnu  -lmysqlcppconn


all: mySqlSelect

mySqlSelect: mySqlSelect.o
    $(CC) $(LDFLAGS) mySqlSelect.o -o mySqlSelect
mySqlSelect.o: mySqlSelect.cpp
    $(CC) $(CFLAGS) mySqlSelect.cpp -o  mySqlSelect.o
