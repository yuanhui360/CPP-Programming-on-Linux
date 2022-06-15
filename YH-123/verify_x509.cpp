#include "x509_certificate.h"

int main(int argc, char* argv[]) {

    X509             *myX509 = NULL;
    EVP_PKEY         *myPkey = NULL;

    if (argc != 3) {
        fprintf(stdout, "Usage   : %s <x509_file> <public_key_file>\n", argv[0]);
        fprintf(stdout, "Example : %s my_x509.pem my_public_key.pem\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    LibOpenSSL::My_X509_Certificate        x509Obj;
    myX509 = x509Obj.load_x509_certificate( argv[1]);
    if ( myX509 == nullptr ) {
        std::cout << "Load X509 Certificate Error" << std::endl;
        exit(EXIT_FAILURE);
    }
  
    myPkey = x509Obj.load_key( argv[2], false); // <- Load Public Key
    if ( myPkey == nullptr ) {
        std::cout << "Load Public file Error" << std::endl;
        exit(EXIT_FAILURE);
    }

    int rc = X509_verify(myX509, myPkey);
    if ( rc != 1 ) {
        std::cout << "X509_verify()  Error" << std::endl;
    }
    else {
        std::cout << "X509_verify()  Success" << std::endl;
    }

    return 0;
}
