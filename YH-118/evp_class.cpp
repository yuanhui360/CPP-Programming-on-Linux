#include "evp_class.h"

using namespace LibOpenSSL;

AES_CBC_256::AES_CBC_256() {

    key = (unsigned char *)"01234567890123456789012345678901"; // A 256 bit key 32 bytes
    iv =  (unsigned char *)"0123456789012345";                 // A 128 bit IV  16 bytes
}

int AES_CBC_256::encrypt(unsigned char *plaintext, unsigned char *ciphertext) {

    /*
     *    1) encrypt plaintext to ciphertext
     *    2) return integer of ciphertext length
     *    3) using EVP_aes_256_cbc() algorithm
     *
     *    AES — Advanced Encryption Standard
     *    CBC - Cipher Block Chaining mode
     */

    int len;
    int plaintext_len = strlen((char*)plaintext);
    int ciphertext_len;
    int rc;

    /*
     *   EVP_CIPHER_CTX_new() creates a cipher context.
     */

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    /*
     *  EVP_EncryptInit_ex() sets up cipher context ctx for encryption with cipher
     *  type from ENGINE impl. ctx must be created before calling this function.
     *
     *  Parameters:
     *    1) ctx must be created before calling this function.
     *    2) Type is normally supplied by a function such as EVP_aes_256_cbc().
     *    3) If impl is NULL then the default implementation is used.
     *    4) key is the symmetric key
     *    5) iv the initialization vector IV as hexadecimal number
     */

    rc = EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    if(rc != ERR_LIB_NONE) {
        std::cout << "encrypt: EVP_DecryptInit_ex() error" << std::endl;
        return -1;
    }

    /*
     *  EVP_EncryptUpdate() encrypts n bytes from the buffer in and writes
     *  the encrypted version to out.
     *  This function can be called multiple times to encrypt successive blocks
     *  of data.
     */

    rc = EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    if(rc != ERR_LIB_NONE) {
        std::cout << "encrypt: EVP_EncryptUpdate() error" << std::endl;
        return -1;
    }
    ciphertext_len = len;

    /*
     *   EVP_EncryptFinal_ex() encrypts the "final" data, that is any data that remains
     *   in a partial block
     */

    rc = EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    if(rc != ERR_LIB_NONE) {
        std::cout << "encrypt: EVP_EncryptFinal_ex() error" << std::endl;
        return -1;
    }
    ciphertext_len += len;

    /*
     *  EVP_CIPHER_CTX_free() clears all information from a cipher context and free up
     *  any allocated memory associate with it, including ctx itself.
     */

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int AES_CBC_256::decrypt(unsigned char *plaintext, unsigned char *ciphertext, int ciphertext_len) {

    /*
     *    1) decrypt ciphertext to plaintext
     *    2) return integer of plaintext length
     *    3) using EVP_aes_256_cbc() algorithm
     */

    int len;
    int plaintext_len;
    int rc;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    rc = EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    if(rc != ERR_LIB_NONE) {
        std::cout << "decrypt: EVP_DecryptInit_ex() error" << std::endl;
        return -1;
    }

    rc = EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    if(rc != ERR_LIB_NONE) {
        std::cout << "decrypt: EVP_DecryptUpdate() error" << std::endl;
        return -1;
    }
    plaintext_len = len;

    rc = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    if(rc != ERR_LIB_NONE) {
        std::cout << "decrypt: EVP_DecryptFinal_ex() error" << std::endl;
        return -1;
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

void AES_CBC_256::print_ciphertext( unsigned char *ciphertext, int ciphertext_len ) {

    /*
     *  1) Print ciphertext on stdout in hex format
     */

    printf("Ciphertext is:\n");
    BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);
}

AES_CBC_256::~AES_CBC_256() {
}

