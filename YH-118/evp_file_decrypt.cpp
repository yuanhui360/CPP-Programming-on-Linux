#include "evp_class.h"

int main(int argc, char* argv[]) {

    unsigned char ciphertext[MAX_BUFFER_SIZE];
    unsigned char plaintext[MAX_BUFFER_SIZE];

    int inBytes, outBytes;
    int plaintext_len;
    BIO  *bio_in = NULL;
    BIO *bio_out = NULL;

    memset(plaintext, '\0', MAX_BUFFER_SIZE);
    memset(ciphertext, '\0', MAX_BUFFER_SIZE);

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <encrypted file> <plaintext file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    bio_in  = BIO_new_file(argv[1], "r");
    bio_out = BIO_new_file(argv[2], "w");

    LibOpenSSL::AES_CBC_256      encObj = LibOpenSSL::AES_CBC_256();
    while ((inBytes = BIO_read(bio_in, ciphertext, MAX_BUFFER_SIZE)) > 0 ) {
        plaintext_len = encObj.decrypt(plaintext, ciphertext, inBytes);
        if ( plaintext_len > 0 ) {
            outBytes = BIO_write(bio_out, plaintext, plaintext_len);
            if ( outBytes != plaintext_len ) {
                std::cout << "In Bytes : " << plaintext_len << " Out Bytes : " << outBytes << std::endl;
                break;
            }
        }
    }

    BIO_free(bio_in);
    BIO_free(bio_out);

    return 0;
}
