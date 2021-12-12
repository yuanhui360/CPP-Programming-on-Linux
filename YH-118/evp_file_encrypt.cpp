#include "evp_class.h"

int main(int argc, char* argv[]) {

    unsigned char ciphertext[MAX_BUFFER_SIZE];
    unsigned char plaintext[MAX_BUFFER_SIZE];
    int inBytes, outBytes;
    int ciphertext_len;
    BIO  *bio_in = NULL;
    BIO *bio_out = NULL;

    memset(plaintext, '\0', MAX_BUFFER_SIZE);
    memset(ciphertext, '\0', MAX_BUFFER_SIZE);

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source file> <encrypted file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    bio_in = BIO_new_file(argv[1], "r");
    bio_out = BIO_new_file(argv[2], "w");
    LibOpenSSL::AES_CBC_256   encObj = LibOpenSSL::AES_CBC_256();
    while ( (inBytes = BIO_read(bio_in, plaintext, MAX_BUFFER_SIZE)) > 0 ) {
        ciphertext_len = encObj.encrypt(plaintext, ciphertext);
        if ( ciphertext_len > 0 ) {
            outBytes = BIO_write(bio_out, ciphertext, ciphertext_len);
            if ( outBytes != ciphertext_len ) {
                std::cout << "In Bytes : " << ciphertext_len << " Out Bytes : " << outBytes << std::endl;
                break;
            }
        }
    }
    BIO_free(bio_in);
    BIO_free(bio_out);

    return 0;
}
