CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -laws-cpp-sdk-core -laws-cpp-sdk-lambda

all: invoke_function

invoke_function: invoke_function.o
	$(CC) $(LDFLAGS) invoke_function.o -o invoke_function
invoke_function.o: invoke_function.cpp
	$(CC) $(CFLAGS) invoke_function.cpp

clean:
	rm -f *.o invoke_function
