#ifndef EVP_CLASS_H
#define EVP_CLASS_H

#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <sqlite3.h>
#include <string.h>
#include <sstream>

#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#define MAX_BUFFER_SIZE    512

/*
 *  In order to perform encryption/decryption we need to select 4 componentes:
 *
 *  algorithm : Ciphers. AES, DES, RC2, RC4, RC5, Triple DES ...
 *       mode : GCM, CCM, CFB, ECB, ...
 *        key : the key used for encryption or decryption by the cipher algorithm.
 *        iv  : the initialization vector IV as hexadecimal number.
 */

namespace LibOpenSSL {

    /*
     *    AES — Advanced Encryption Standard
     *    CBC - Cipher Block Chaining mode
     */

    class AES_CBC_256 {
    public:
        AES_CBC_256();

        int encrypt(unsigned char *plaintext, unsigned char *ciphertext);
        int decrypt(unsigned char *plaintext, unsigned char *ciphertext, int ciphertext_len);
        void print_ciphertext( unsigned char *ciphertext, int ciphertext_len );

        ~AES_CBC_256();

    private:
        unsigned char *key;
        unsigned char *iv;
    };

};

#endif // SQLITE_TEMPLATE_H
    
