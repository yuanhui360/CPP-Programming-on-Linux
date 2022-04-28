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

#define MAX_BUFFER_SIZE         512
#define MAX_SIGN_BUFFER_SIZE    1024

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

        /*
         *   A digital signature consists of three algorithms;
         *
         *     1) A key generation algorithm that outputs the private key and a corresponding public key.
         *     2) A signing algorithm that, given a message and a private key, produces a signature.
         *     3) A signature verifying algorithm that, given the message, public key and signature,
         *        either accepts or rejects the message's claim to authenticity.
         *
         *   EVP_PKEY_sign   - sign using a public key algorithm (generate signature using private key)
         *   EVP_PKEY_verify - signature verification using a public key algorithm
         *
         *   Example:
         *     1) Bob plan to sign document and send to Alice
         *     2) Bob generate private key and public key
         *     3) Bob create digital signature using his private key and send to Alice
         *     4) Alice received sugnature file and using Bob's public key to verify
         *     5) If digital verify OK, do something, if not OK, do something else
         */

        unsigned int load_to_md(unsigned char *md_value, std::string file_name);
        int pkey_sign(std::string Orig_file, std::string sign_file);
        int pkey_verfiy(std::string Orig_file, std::string sign_file);

        ~MyEVP_Key();

    private:
        EVP_PKEY      *pkey;
    };

};

#endif // SQLITE_TEMPLATE_H
