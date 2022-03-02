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
#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/dsa.h>

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

    struct Message {
        size_t      msg_len;
        unsigned char msg_body[MAX_BUFFER_SIZE];
        Message() {
            msg_len = 0;
            memset(msg_body, '\0', MAX_BUFFER_SIZE);
        }
    };

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

    class MyEVP_Key {
    public:
         MyEVP_Key();

        EVP_PKEY* generate_rsa_key(unsigned int key_bits);
        EVP_PKEY* load_key(std::string key_file_type, std::string key_file);

        EVP_PKEY* get_pkey() { return pkey; }
        int  get_pkey_size() { return EVP_PKEY_size(pkey); }
        int  get_pkey_bits() { return EVP_PKEY_bits(pkey); }

        std::string get_keytype();
        void display_key(bool is_private = true);
        void write_key(std::string key_file, bool is_private = true);

        void print_hash(unsigned char *value, size_t length);

        /*
         *   Text Book RSA Key pair Example:  Bob plan to send secrit message to Alice;
         *
         *     1) Alice send her public key to bob, and keep private key in herself
         *     2) Bon encrypted message using public key and send encrypted message to Alice
         *
         *     3) Alice received encrypted message and decrypted hash message using private key
         *     4) afetr decryption, she is able to print/read decrypted message (in text)
         */

        int pkey_encrypt(const unsigned char *pInText, unsigned char *pOutBytes);
        int pkey_decrypt(unsigned char *pOutText, const unsigned char *pInBytes, size_t inBytes_len);

        ~MyEVP_Key();

    private:
        EVP_PKEY    *pkey; 
    };
};

#endif // SQLITE_TEMPLATE_H
