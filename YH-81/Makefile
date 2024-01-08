CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -laws-cpp-sdk-core -laws-cpp-sdk-lambda -laws-cpp-sdk-sqs

all: sqs_recvMsg sqs_sendMsg

sqs_recvMsg: sqs_recvMsg.o
	$(CC) $(LDFLAGS) sqs_recvMsg.o -o sqs_recvMsg
sqs_recvMsg.o: sqs_recvMsg.cpp
	$(CC) $(CFLAGS) sqs_recvMsg.cpp

sqs_sendMsg: sqs_sendMsg.o
	$(CC) $(LDFLAGS) sqs_sendMsg.o -o sqs_sendMsg
sqs_sendMsg.o: sqs_sendMsg.cpp
	$(CC) $(CFLAGS) sqs_sendMsg.cpp

clean:
	rm -f *.o sqs_recvMsg sqs_sendMsg

