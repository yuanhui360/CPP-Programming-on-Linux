#include "x509_certificate.h"

using namespace LibOpenSSL;

My_X509_Certificate::My_X509_Certificate() : p_x509(nullptr), p_pkey(nullptr) {}

EVP_PKEY* My_X509_Certificate::generate_rsa_key(unsigned int key_bits) {

    /*
     *   Methodon : generate RSA EVP_PKEY, and update p_pkey
     *   Parameter:
     *   Input    : key_bits
     *   return   : pointer of EVP_PKEY
     */

    int rc;

    /*  Step 1 : Initancialize EVP_PKEY Object, allocate memory for p_pkey*/

    p_pkey = EVP_PKEY_new();
    if ( p_pkey == nullptr ) {
        std::cout << "generate_rsa_key->EVP_PKEY_new() error" << std::endl;;
        return NULL;
    }

    /*  Step 2 : Create EVP_PKEY_CTX object,  allocate memory for ctx */

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if ( ctx == nullptr ) {
        std::cout << "generate_rsa_key->EVP_PKEY_CTX_new_id() error" << std::endl;;
        return NULL;
    }
 
    /*  Step 3 : Initializ ctx object */

    rc = EVP_PKEY_keygen_init(ctx);
    if ( rc != 1 ) {
        std::cout << "generate_rsa_key->EVP_PKEY_keygen_init() error" << std::endl;;
        return NULL;
    }

    /*  Step 4 : sets the RSA key bits. If not specified 1024 bits is used.  */

    rc = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, key_bits);
    if ( rc <= 0 ) {
        std::cout << "generate_rsa_key->EVP_PKEY_CTX_set_rsa_keygen_bits() error" << std::endl;;
        return NULL;
    }

    /*  Step 5 : Generate Key */

    rc = EVP_PKEY_keygen(ctx, &p_pkey);
    if ( rc != 1 ) {
        std::cout << "generate_rsa_key->EVP_PKEY_keygen() error" << std::endl;;
        return NULL;
    }

    /*  Step 6 : free ctx allocated memory */

    EVP_PKEY_CTX_free(ctx);

    return p_pkey;
}

int My_X509_Certificate::save_pkey( std::string key_file, bool is_private ) {

    /*
     *   Method    : Save p_pkey (EVP_PKEY) to key_file,
     *   parameter : key_file   <- The key file name save the key to
     *               is_private =  true,  save to private key file
     *                             false, save to public key file
     *   Return : The write routines return 1 for success or 0 for failure.
     */

    int rc;
    if ( p_pkey == nullptr ) {
        std::cout << "save_pkey->p_pkey is not generated or loaded " << std::endl;;
        return 0;
    }

    BIO *bio_out = BIO_new_file(key_file.c_str(), "w");
    if ( is_private )
        rc = PEM_write_bio_PrivateKey(bio_out, p_pkey, NULL, NULL, 0, 0, NULL);
    else
        rc = PEM_write_bio_PUBKEY(bio_out, p_pkey);
    BIO_free(bio_out);
    return rc;

}

EVP_PKEY* My_X509_Certificate::load_key(std::string key_file, bool is_private) {

    /*
     *  Method     : Load from key_file to p_pkey (EVP_PKEY)
     *  Parameter  :
     *  Input      : key_file   <- The key file that load from
     *               is_private =  true,  load from private key file
     *                             false, load from public key file
     *  Return     : pointer of EVP_PKEY
     */

    BIO *bio_key = NULL;

    if ( p_pkey != nullptr ) {
        EVP_PKEY_free(p_pkey);
    }

    bio_key = BIO_new_file(key_file.c_str(), "r");

    if (is_private ) {
        p_pkey = PEM_read_bio_PrivateKey(bio_key, NULL, NULL, NULL);
    }
    else {
        p_pkey = PEM_read_bio_PUBKEY(bio_key, NULL, NULL, NULL);
    }
    if (p_pkey == nullptr) {
        std::cout << "My_X509_Certificate->load_key() Error" << key_file << std::endl;
    }

    BIO_free(bio_key);

    return p_pkey;
}


X509* My_X509_Certificate::generate_x509( unsigned int key_bits ) {

    /*
     *   Method    : Generate x509 certificate and EVP_PKEY
     *               update p_x509 and p_pkey
     *   Parameter :
     *   Input     : key_bits for generate EVP_PKEY
     *   Return    : pointer of x509 certificate (p_x509)
     */

    int rc;

    /* check if p_x509 has been generated */
    if ( p_x509 != nullptr ) {
        std::cout << "X509 Certificate has alread generated or loaded" << std::endl;
        return p_x509;
    }

    if ( p_pkey != nullptr ) {
        EVP_PKEY_free(p_pkey);
    }

    p_pkey = generate_rsa_key(key_bits);
    if ( p_pkey == nullptr ) {
        std::cout << "generate_x509()->generate_rsa_key() error " << std::endl;
        return NULL;
    }

    /* Instancialize and allocate memmory for x509 object*/
    p_x509 = X509_new();
    if ( p_x509 == nullptr ) {
        std::cout << "generate_x509()->X509_new()() error " << std::endl;
        return NULL;
    }

    /*
     *  X.509 is an International Telecommunication Union (ITU) standard defining
     *        the format of public key certificates.
     *  The structure of an X.509 v3 digital certificate is as follows:
     *    Certificate
     *        Version Number
     *        Serial Number
     *        Signature Algorithm ID
     *        Issuer Name
     *        Validity period
     *            Not Before
     *            Not After
     *        Subject name
     *        Subject Public Key Info
     *        Public Key Algorithm
     *        Subject Public Key
     *        Issuer Unique Identifier (optional)
     *        Subject Unique Identifier (optional)
     *        Extensions (optional)
     *           ...
     *        Certificate Signature Algorithm
     *        Certificate Signature
     *
     */

    /* Set the version number. int X509_set_version(X509 *x, long version); */

    rc = X509_set_version(p_x509, 1L);
    if ( rc != 1 ) {
        std::cout << "generate_x509->X509_set_version() error" << std::endl;
    }

    /*  Set the serial number.
     *  int ASN1_INTEGER_set(const ASN1_INTEGER *a, long v);
     *  ASN1_INTEGER *X509_get_serialNumber(X509 *x); it internal pointer
     */
    ASN1_INTEGER_set(X509_get_serialNumber(p_x509), 1L);

    /* set validation period,
     *    ASN1_TIME *X509_time_adj(ASN1_TIME *s, long adj, time_t *t);
     *    ASN1_TIME *X509_gmtime_adj(ASN1_TIME *s, long adj);
     *
     *    time_t in_tm;
     *    in_tm = time(NULL);    // <- get current time
     *    X509_time_adj(X509_get_notBefore(p_x509), 0L, &in_tm);
     *    X509_time_adj(X509_get_notAfter(p_x509), 31536000L, &in_tm);
     */

     X509_gmtime_adj(X509_get_notBefore(p_x509), 0L);
     X509_gmtime_adj(X509_get_notAfter(p_x509), 31536000L);  // <- 365x24x60x60 seconds

    /* Setup subject name. */
    X509_NAME *p_name = X509_NAME_new();
    if (p_name == nullptr ) {
        std::cout << "generate_x509->X509_NAME_new() error" << std::endl;
        return NULL;
    }

    /*
     *  Typical information for Subject and Issuer
     *
     *    CN    : Common Name
     *    O     : Organization Name
     *    OU    : Organizational Unit
     *    L     : Locality
     *    ST    : State/Province
     *    C     : Country
     *    EMAIL : Email Address
     */

    X509_NAME_add_entry_by_txt(p_name, "C",  MBSTRING_ASC, (unsigned char *)"CA",             -1, -1, 0);
    X509_NAME_add_entry_by_txt(p_name, "ST", MBSTRING_ASC, (unsigned char *)"Ontario",        -1, -1, 0);
    X509_NAME_add_entry_by_txt(p_name, "L",  MBSTRING_ASC, (unsigned char *)"Toronto",        -1, -1, 0);
    X509_NAME_add_entry_by_txt(p_name, "O",  MBSTRING_ASC, (unsigned char *)"My Company",     -1, -1, 0);
    X509_NAME_add_entry_by_txt(p_name, "OU", MBSTRING_ASC, (unsigned char *)"My Department",  -1, -1, 0);
    X509_NAME_add_entry_by_txt(p_name, "CN", MBSTRING_ASC, (unsigned char *)"localhost",      -1, -1, 0);
    X509_NAME_add_entry_by_txt(p_name, "CN", MBSTRING_ASC, (unsigned char *)"CA Issuer Name", -1, -1, 0);

    /* Set subject name. Certificat who belone to*/
    rc = X509_set_subject_name(p_x509, p_name);
    if ( rc != 1 ) {
        std::cout << "generate_x509->X509_set_subject_name() error" << std::endl;
        return NULL;
    }

    /* set issuer name. Certificate who signed*/
    rc = X509_set_issuer_name(p_x509, p_name);
    if ( rc != 1 ) {
        std::cout << "generate_x509->X509_set_issuer_name() error" << std::endl;
        return NULL;
    }

    /* Set the public key for certificate. */
    X509_set_pubkey(p_x509, p_pkey);

    /* Actually sign the certificate with key and algorithm. */
    rc = X509_sign(p_x509, p_pkey, EVP_sha256());
    if ( rc <= 0 ) {
        std::cout << "generate_x509->X509_sign() error" << std::endl;
        return NULL;
    }
    else {
        std::cout << "X509 signature length : " << rc << std::endl;
    }

    return p_x509;
}

    /*
     *  X509 *PEM_read_bio_X509(BIO *bp, X509 **x, pem_password_cb *cb, void *u);
     *
     *  The cb argument is the callback to use when querying for the pass phrase used
     *  for encrypted PEM structures (normally only private keys).
     *
     *  The callback routine has the following form:
     *     int cb(char *buf, int size, int rwflag, void *u);
     *
     *  If the cb parameters is set to NULL and the u parameter is not NULL then
     *  the u parameter is interpreted as a null terminated string to use as
     *  the passphrase.
     *
     *  If both cb and u are NULL then the default callback routine is used which will
     *  typically prompt for the passphrase on the current terminal with echoing turned off.
     *
     */

X509* My_X509_Certificate::load_x509_certificate( std::string x509_file ) {

    /*
     *   Method  : Load x509 certificate from certificate file to p_x509
     *             Extract public key to p_pkey
     *   Input   : x509_file
     *   Return  : Pointer of X509 certificate
     */

    BIO *bio_x509 = NULL;
    bio_x509 = BIO_new_file(x509_file.c_str(), "r");

    /* Instancialize x509 object and allocate memmory */
    if( p_x509 != nullptr )
        X509_free(p_x509);

    /*
     *  X509 *PEM_read_bio_X509(BIO *bp, X509 **x, pem_password_cb *cb, void *u)
     */
    p_x509 = PEM_read_bio_X509(bio_x509, NULL, NULL, NULL);

    BIO_free(bio_x509);
    if (p_x509 == nullptr) {
        std::cout << "load_x509_certificate->PEM_read_bio_X509() Error" << x509_file << std::endl;
        return NULL;
    }

    /* Extract public key to p_pkey */
    p_pkey = X509_get_pubkey(p_x509);
    if (p_pkey == nullptr) {
        std::cout << "load_x509_certificate->X509_get_pubkey() Error" << x509_file << std::endl;
    }

    int rc = X509_verify(p_x509, p_pkey);
    if ( rc != 1 ) {
        std::cout << "load_x509_certificate->X509_verify() Error : " << x509_file << std::endl;
    }
    else {
        std::cout << "load_x509_certificate->X509_verify() Success : " << x509_file << std::endl;
    }

    return p_x509;
}

int My_X509_Certificate::save_x509_certificate(std::string file_name ) {

    /*
     *    Method    : Save x509 certificate to file
     *    Parameter : file_name <- file the x509 is going to save
     *    Return    : 1 success , 0 or negative fail
     */

    int rc = 0;
    BIO *bio_out = NULL;
    if ( p_x509 == nullptr ) {
        std::cout << "X509 Certificate has not been generated or loaded " << std::endl;
        return -1;
    }

    bio_out = BIO_new_file(file_name.c_str(), "w");

    rc =  PEM_write_bio_X509(bio_out, p_x509);
    if ( rc != 1 ) {
        std::cout << "save_x509_certificate->PEM_write_bio_X509() error " << std::endl;
        return -1;
    }

    BIO_free(bio_out);
    return rc;
}

My_X509_Certificate::~My_X509_Certificate() {
    if( p_x509 != nullptr )
        X509_free(p_x509);
    if( p_pkey != nullptr )
        EVP_PKEY_free(p_pkey);
}
                                                 
