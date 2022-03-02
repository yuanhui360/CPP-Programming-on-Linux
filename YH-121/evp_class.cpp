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

// ************ Start MyEVP_Key class **************************

MyEVP_Key::MyEVP_Key() : pkey(nullptr) {}

EVP_PKEY* MyEVP_Key::generate_rsa_key(unsigned int key_bits) {

    /*
     *   Step 1 : Create EVP_PKEY_CTX object
     *            EVP_PKEY_CTX *EVP_PKEY_CTX_new_id(int id, ENGINE *e);
     *   Step 2 : Initializa ctx object
     *            EVP_PKEY_keygen_init(EVP_PKEY_CTX *ctx);
     *   Step 3 : The EVP_PKEY_CTX_set_rsa_keygen_bits() macro sets the RSA
     *            key length for RSA key generation to bits. If not specified
     *            1024 bits is used.
     *   Step 4 : Generate Key
     *            EVP_PKEY_keygen(EVP_PKEY_CTX *ctx, EVP_PKEY **ppkey);
     *   Step 5 : free ctx EVP_PKEY_CTX_free(EVP_PKEY_CTX *ctx)
     */

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY_keygen_init(ctx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, key_bits);
    EVP_PKEY_keygen(ctx, &pkey);
    EVP_PKEY_CTX_free(ctx);

    return pkey;
}

EVP_PKEY* MyEVP_Key::load_key( std::string key_file_type, std::string key_file) {

    /*
     *  key_file_type : private  private key
     *                  public   public key
     *
     *  EVP_PKEY *PEM_read_bio_PrivateKey(BIO *bp, EVP_PKEY **x,
     *                             pem_password_cb *cb, void *u);
     *  EVP_PKEY *PEM_read_bio_PUBKEY(BIO *bp, EVP_PKEY **x,
     *                         pem_password_cb *cb, void *u);
     *
     *  The PEM read functions all take an argument TYPE **x and return a TYPE * pointer.
     *  Where TYPE is whatever structure the function uses.
     *
     *  If x is NULL then the parameter is ignored. If x is not NULL but *x is NULL then
     *  the structure returned will be written to *x.
     *
     *  If neither x nor *x is NULL then an attempt is made to reuse the structure at *x
     *  (but see BUGS and EXAMPLES sections). Irrespective of the value of x a pointer to
     *  the structure is always returned (or NULL if an error occurred).
     */

    BIO *bio_key = NULL;
    bio_key = BIO_new_file(key_file.c_str(), "r");

    if (key_file_type == "public" ) {
        pkey = PEM_read_bio_PUBKEY(bio_key, NULL, NULL, NULL);
    }

    if (key_file_type == "private" ) {
        pkey = PEM_read_bio_PrivateKey(bio_key, NULL, NULL, NULL);
    }

    if (pkey == nullptr) {
        std::cout << "Error : failed to load key from " << key_file << std::endl;
    }

    BIO_free(bio_key);

    return pkey;
}

std::string MyEVP_Key::get_keytype() {

    std::string key_type_str;
    int type_id;

    type_id = EVP_PKEY_base_id(pkey);
    switch( EVP_PKEY_type( type_id ) ) {
        case EVP_PKEY_RSA :
            key_type_str = "EVP_PKEY_RSA";
            break;
        case EVP_PKEY_DSA :
            key_type_str = "EVP_PKEY_DSA";
            break;
        case EVP_PKEY_EC :
            key_type_str = "EVP_PKEY_EC";
            break;
        case EVP_PKEY_HMAC :
            key_type_str = "EVP_PKEY_HMAC";
            break;
        default :
            key_type_str = "EVP_PKEY_NONE";
            break;
    }

    return key_type_str;
}

void MyEVP_Key::display_key( bool is_private ) {

    /*
     *   The functions EVP_PKEY_print_public(), EVP_PKEY_print_private() and
     *   EVP_PKEY_print_params() print out the public, private or parameter
     *   components of key pkey respectivel
     *
     *   int EVP_PKEY_print_public(BIO *out, const EVP_PKEY *pkey,
     *                     int indent, ASN1_PCTX *pctx);
     *   int EVP_PKEY_print_private(BIO *out, const EVP_PKEY *pkey,
     *                      int indent, ASN1_PCTX *pctx);
     *
     *   The parameter indent indicates how far the printout should be indented.
     *   The pctx parameter allows the print output to be finely tuned by using ASN1
     *   printing options. If pctx is set to NULL then default values will be used.
     */

    BIO *bio_out =  BIO_new_fp(stdout, BIO_NOCLOSE);
    if ( is_private )
        EVP_PKEY_print_private(bio_out, pkey, 0, NULL);
    else
        EVP_PKEY_print_public(bio_out, pkey, 0, NULL);
    BIO_free(bio_out);
}

void MyEVP_Key::print_hash(unsigned char *value, size_t length) {

     printf("Hash Value : (%d) ", (int)length);
     for (size_t i = 0; i < length; i++)
         printf("%02x:", value[i]);
     printf("\n");
}

void MyEVP_Key::write_key( std::string key_file, bool is_private ) {

    /*
     *   int PEM_write_bio_PrivateKey(BIO *bp, EVP_PKEY *x, const EVP_CIPHER *enc,
     *                        unsigned char *kstr, int klen,
     *                        pem_password_cb *cb, void *u);
     *   int PEM_write_bio_PUBKEY(BIO *bp, EVP_PKEY *x);
     *
     *   typedef int pem_password_cb (char *buf, int size, int rwflag, void *userdata);
     *                  pem_password_cb *callback, void *u);
     */

    BIO *bio_out = BIO_new_file(key_file.c_str(), "w");
    if ( is_private )
        PEM_write_bio_PrivateKey(bio_out, pkey, NULL, NULL, 0, 0, NULL);
    else
        PEM_write_bio_PUBKEY(bio_out, pkey);
    BIO_free(bio_out);
}

int MyEVP_Key::pkey_encrypt(const unsigned char *pInText, unsigned char *pOutBytes) {

    /*
     *  Encrypt using  public key algorithm context using key (pkey)
     *  for an encryption operation.
     *  https://www.openssl.org/docs/manmaster/man3/EVP_PKEY_encrypt_init_ex.html
     *
     *  Encrypted data is written to *pOutBytes
     *
     *  RETURN VALUES
     *  Encrypted data bytes length (postive), 0 or a negative value for failure.
     */

    int rc = 0;
    size_t out_bytes_len;
    size_t in_text_len = (size_t)strlen((char*)pInText);

    if( pkey == nullptr ) {
        std::cout << "pkey has not been generated or loaded" << std::endl;
        return 0;
    }

    /*
     *  Step 1 : new ctx object
     *           EVP_PKEY_CTX *EVP_PKEY_CTX_new(EVP_PKEY *pkey, ENGINE *e);
     */

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if ( ctx == nullptr ) {
        std::cout << "EVP_PKEY_CTX_new() Error" << std::endl;
        rc = -1;
        return rc;
    }
 
    /*
     *  Step 2 : Initializ public key algorithm
     *           int EVP_PKEY_encrypt_init(EVP_PKEY_CTX *ctx)
     */

    rc = EVP_PKEY_encrypt_init(ctx);
    if ( rc <= 0 ) {
        std::cout << "EVP_PKEY_encrypt_init() Error" << std::endl;
        return rc;
    }

    /*
     *  Step 3 : Perform public key encryption
     *           int EVP_PKEY_encrypt(EVP_PKEY_CTX *ctx,
     *                                unsigned char *out, size_t *outlen,
     *                                const unsigned char *in, size_t inlen);
     *
     *  void *OPENSSL_malloc(size_t num)
     *  void  OPENSSL_free(void *addr)
     */

    rc = EVP_PKEY_encrypt(ctx, NULL, &out_bytes_len, pInText, in_text_len);
    if ( rc <= 0 ) {
        std::cout << "EVP_PKEY_encrypt(NULL) Error" << std::endl;
        return rc;
    }

    rc = EVP_PKEY_encrypt(ctx, pOutBytes, &out_bytes_len, pInText, in_text_len);
    if ( rc <= 0 ) {
        std::cout << "EVP_PKEY_encrypt() Error" << std::endl;
        return rc;
     }

    /*
     *  Step 4 : Free ctx object allocated memory
     *           EVP_PKEY_CTX_free(EVP_PKEY_CTX *ctx)
     */

    EVP_PKEY_CTX_free(ctx);
    rc = out_bytes_len;

    return rc;
}

int MyEVP_Key::pkey_decrypt(unsigned char *pOutText, const unsigned char *pInBytes, size_t inBytes_len) {

    /*
     *  Decrypt using public key algorithm context using key pkey
     *  for an decryption operation (pkey).
     *  https://www.openssl.org/docs/manmaster/man3/EVP_PKEY_decrypt.html
     *
     *  RETURN VALUES:
     *  Decrypted data length, 0 or a negative value for failure.
     */

    size_t text_len;
    int rc;

    if( pkey == nullptr ) {
        std::cout << "pkey has not been initialized or loaded" << std::endl;
        return 0;
    }


    /*
     *  Step 1 : New ctx object
     *           EVP_PKEY_CTX *EVP_PKEY_CTX_new(EVP_PKEY *pkey, ENGINE *e);
     */

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if ( ctx == nullptr ) {
        std::cout << "EVP_PKEY_CTX_new() Error" << std::endl;
        rc = -1;
        return rc;
    }

    /*
     *  Step 2 : initializes a public key algorithm context using key pkey
     *           int EVP_PKEY_decrypt_init(EVP_PKEY_CTX *ctx)
     */

    rc = EVP_PKEY_decrypt_init(ctx);
    if ( rc <= 0 ) {
        std::cout << "EVP_PKEY_decrypt_init() Error" << std::endl;
        return rc;
    }

    /*
     *  Step 3 : performs a public key decryption operation using ctx.
     *           int EVP_PKEY_decrypt(EVP_PKEY_CTX *ctx,
     *                                unsigned char *out, size_t *outlen,
     *                                const unsigned char *in, size_t inlen);
     */

    rc = EVP_PKEY_decrypt(ctx, NULL, &text_len, pInBytes, inBytes_len);
     if ( rc <= 0 ) {
        std::cout << "EVP_PKEY_decrypt(NULL) Error" << std::endl;
        return rc;
    }

    rc = EVP_PKEY_decrypt(ctx, pOutText, &text_len, pInBytes, inBytes_len);
    if ( rc <= 0 ) {
        std::cout << "EVP_PKEY_decrypt() Error" << std::endl;
        return rc;
    }

    /*
     *  Step 4 : free ctx object (allocated memory)
     */

    EVP_PKEY_CTX_free(ctx);

    rc = text_len;
    return rc;
}

MyEVP_Key::~MyEVP_Key() {

    if( pkey != nullptr )
        EVP_PKEY_free(pkey);
}
