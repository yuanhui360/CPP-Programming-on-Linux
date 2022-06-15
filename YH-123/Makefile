CC=g++
CFLAGS=-c -Wall -g
LDFLAGS = -lcrypto -lssl -lrt -lpthread -lboost_system

all: generate_x509 verify_x509 display_x509

generate_x509: generate_x509.o x509_certificate.o
    $(CC) $(LDFLAGS) generate_x509.o x509_certificate.o -o generate_x509
generate_x509.o: generate_x509.cpp
    $(CC) $(CFLAGS) generate_x509.cpp -o generate_x509.o

verify_x509: verify_x509.o x509_certificate.o
    $(CC) $(LDFLAGS) verify_x509.o x509_certificate.o -o verify_x509
verify_x509.o: verify_x509.cpp
    $(CC) $(CFLAGS) verify_x509.cpp -o verify_x509.o

display_x509: display_x509.o x509_certificate.o
    $(CC) $(LDFLAGS) display_x509.o x509_certificate.o -o display_x509
display_x509.o: display_x509.cpp
    $(CC) $(CFLAGS) display_x509.cpp -o display_x509.o

x509_certificate.o: x509_certificate.cpp
    $(CC) $(CFLAGS) x509_certificate.cpp -o x509_certificate.o

clean:
    rm -f *.o generate_x509 verify_x509 display_x509
