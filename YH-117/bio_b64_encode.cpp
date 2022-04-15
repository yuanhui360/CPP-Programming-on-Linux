#include <iostream>
#include <stdio.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>

#define  MAX_BUFFER_SIZE      512

/*
 *  BIO_f_base64
 *  BIO_f_base64() is a filter BIO that returns the base64 BIO method.
 *
 *   1) encodes any data written through it
 *   2) decodes any data read through it.
 *
 */

int main(int argc, char *argv[]) {

    BIO *bio_out = NULL;
    BIO *bio_in = NULL;
    BIO *bio_b64 = NULL;
    int inByte, outByte;
    char    buffer[MAX_BUFFER_SIZE];
    memset(buffer, '\0', MAX_BUFFER_SIZE);

    if ( argc != 3)
    {
        std::cout << " Usage : bio_b64_encode <source file> <encoded file>" << std::endl;
        return 1;
    }

    bio_in  = BIO_new_file(argv[1], "r");
    bio_out = BIO_new_file(argv[2], "wb");

    bio_b64 = BIO_new(BIO_f_base64());
    BIO_push( bio_b64, bio_out);

    while ((inByte = BIO_read(bio_in, buffer, MAX_BUFFER_SIZE)) > 0) {
        outByte = BIO_write(bio_b64, buffer, inByte);
        if ( inByte != outByte ) {
            std::cout << "In Bytes : " << inByte << " Out Bytes : " << outByte << std::endl;
            break;
        }
    }

    BIO_free(bio_in);
    
    /*
     *  BIO_flush() normally writes out any internally buffered data, in some cases it is used to signal EOF and that no more data will be written.
     */
    
    BIO_flush(bio_b64);
    BIO_free_all(bio_b64);
    return 0;
}
 
