#include <iostream>
#include <stdio.h>
#include <string.h>

#include <openssl/ssl.h>
#include <openssl/bio.h>

#define  MAX_BUFFER_SIZE      512

int main(int argc, char *argv[]) {

    BIO *bio_out = NULL;
    BIO *bio_in = NULL;
    int inByte, outByte;
    char    buffer[MAX_BUFFER_SIZE];
    memset(buffer, '\0', MAX_BUFFER_SIZE);

    if ( argc != 3)
    {
        std::cout << " Usage : bio_file <file-read> <file-write>" << std::endl;
        return 1;
    }

    bio_in = BIO_new_file(argv[1], "r");
    bio_out = BIO_new_file(argv[2], "w");
    while ((inByte = BIO_read(bio_in, buffer, MAX_BUFFER_SIZE)) > 0 ) {
        outByte = BIO_write(bio_out, buffer, inByte);
        if ( inByte != outByte ) {
            std::cout << "In Bytes : " << inByte << " Out Bytes : " << outByte << std::endl;
            break;
        }
    }

    BIO_free(bio_in);
    BIO_free(bio_out);

    return 0;
}
 
