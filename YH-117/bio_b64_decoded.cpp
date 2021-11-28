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
    int inBytes, outBytes;
    char    buffer[MAX_BUFFER_SIZE];
    memset(buffer, '\0', MAX_BUFFER_SIZE);

    if ( argc != 3)
    {
        std::cout << " Usage : bio_b64_decoded <encoded file> <source file>" << std::endl;
        return 1;
    }

    bio_in  = BIO_new_file(argv[1], "rb");
    bio_out = BIO_new_file(argv[2], "w");

    bio_b64 = BIO_new(BIO_f_base64());
    BIO_push(bio_b64, bio_in);

    while ((inBytes = BIO_read(bio_b64, buffer, MAX_BUFFER_SIZE)) > 0) {
        outBytes = BIO_write(bio_out, buffer, inBytes);
        if (inBytes != outBytes ) {
            std::cout << "In Bytes : " << inBytes << " Out Bytes : " << outBytes << std::endl;
            break;
        }
    }

    BIO_free(bio_out);
    BIO_free_all(bio_b64);
    return 0;
}
 
