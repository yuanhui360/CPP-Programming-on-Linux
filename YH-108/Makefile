CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -pthread

all: recv_signal send_signal my_signal pthread_signal

recv_signal: recv_signal.o
	$(CC) recv_signal.o -o recv_signal $(LDFLAGS)
recv_signal.o: recv_signal.cpp
	$(CC) $(CFLAGS) recv_signal.cpp -o recv_signal.o

send_signal: send_signal.o
	$(CC) send_signal.o -o send_signal $(LDFLAGS)
send_signal.o: send_signal.cpp
	$(CC) $(CFLAGS) send_signal.cpp -o send_signal.o

my_signal: my_signal.o
	$(CC) my_signal.o -o my_signal $(LDFLAGS)
my_signal.o: my_signal.cpp
	$(CC) $(CFLAGS) my_signal.cpp -o my_signal.o

pthread_signal: pthread_signal.o
	$(CC) pthread_signal.o -o pthread_signal $(LDFLAGS)
pthread_signal.o: pthread_signal.cpp
	$(CC) $(CFLAGS) pthread_signal.cpp -o pthread_signal.o

clean:
	rm -f *.o recv_signal send_signal my_signal pthread_signal
