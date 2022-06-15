#ifndef X509_CERTIFICATE_H
#define X509_CERTIFICATE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <string.h>
#include <sstream>

#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/dsa.h>
#include <openssl/pem.h>

namespace LibOpenSSL {

    /*
     *  PKI - public key infrastructure
     *  CSR - certificate Signing Request
     *  CA  - Certificate Authority
     *  RSA - (Rivest–Shamir–Adleman) is a public-key crypto system that is widely used
     *        for secure data transmission.
     *  PEM - Privacy Enhanced Mail File Format:
     *  DER - Distinguished Encoding Rules File Format:
     *
     */

  class My_X509_Certificate {

    public:

        My_X509_Certificate();

        /*
         *
         *  Typical information required in a CSR
         *
         *    CN    : Common Name
         *    O     : Organization Name
         *    OU    : Organizational Unit
         *    L     : Locality
         *    ST    : State
         *    C     : Country
         *    EMAIL : Email Address
         */

        EVP_PKEY* generate_rsa_key(unsigned int key_bits = 2048);
        int       save_pkey(std::string key_file, bool is_private = true);
        EVP_PKEY* load_key(std::string key_file, bool is_private = true);

        /*
         *  X.509 is an International Telecommunication Union (ITU) standard defining
         *        the format of public key certificates.
         */

        X509*   generate_x509( unsigned int key_bits = 2048);
        X509*   load_x509_certificate(std::string cert_file );
        int     save_x509_certificate(std::string cert_file );

        ~My_X509_Certificate();

    private:

        X509        *p_x509;
        EVP_PKEY    *p_pkey;

    };

};

#endif // SQLITE_TEMPLATE_H
 
