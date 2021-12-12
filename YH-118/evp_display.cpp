#include "evp_class.h"

int main(int argc, char* argv[]) {

    unsigned char ciphertext[MAX_BUFFER_SIZE];
    unsigned char plaintext[MAX_BUFFER_SIZE];
    memset(plaintext, '\0', MAX_BUFFER_SIZE);
    memset(ciphertext, '\0', MAX_BUFFER_SIZE);
    int plaintext_len;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source data>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    plaintext_len = strlen(argv[1]);
    memcpy(plaintext, argv[1], plaintext_len);
    std::cout << "Source Text is : " << std::endl;
    std::cout << plaintext << std::endl;

    LibOpenSSL::AES_CBC_256    encObj = LibOpenSSL::AES_CBC_256();
    int ciphertext_len = encObj.encrypt(plaintext, ciphertext);
    if ( ciphertext_len > 0 ) {
        encObj.print_ciphertext(ciphertext, ciphertext_len);
    }

    return 0;
}
