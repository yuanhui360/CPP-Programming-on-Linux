#include "x509_certificate.h"

int main(int argc, char* argv[]) {

    X509             *myX509 = NULL;
    unsigned int     myKeyBits;
    int rc;

    if (argc != 5) {
        fprintf(stdout, "Usage   : %s <bits> <x509_file> <private key file> <public key file>\n", argv[0]);
        fprintf(stdout, "Example : %s 2048 my_x509.pem my_private_key.pem my_public_key.pem\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    myKeyBits = (unsigned int) atoi(argv[1]);
    LibOpenSSL::My_X509_Certificate        x509Obj;
    myX509 = x509Obj.generate_x509(myKeyBits);
    if ( myX509 == nullptr ) {
        std::cout << "generate X509 Certificate Error" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "generate X509 Certificate Success" << std::endl;
    }

    rc = x509Obj.save_x509_certificate(argv[2]); // <- save my_x509.pem
    if ( rc != 1 ) {
        std::cout << "Save X509 Certificate Error" << std::endl;
        exit(EXIT_FAILURE);
    }

    rc = x509Obj.save_pkey(argv[3], true); // <- save my_rsa_private_key.pem
    if ( rc != 1 ) {
        std::cout << "Save Private key Error" << std::endl;
        exit(EXIT_FAILURE);
    }
    rc = x509Obj.save_pkey(argv[4], false); // <- save my_rsa_public_key.pem
    if ( rc != 1 ) {
        std::cout << "Save Public key Error" << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}
   
