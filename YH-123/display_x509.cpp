#include "x509_certificate.h"

int main(int argc, char* argv[]) {

    X509             *myX509 = NULL;

    if (argc != 2) {
        fprintf(stdout, "Usage   : %s <x509_file>\n", argv[0]);
        fprintf(stdout, "Example : %s my_x509.pem\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    LibOpenSSL::My_X509_Certificate             x509Obj;
    myX509 = x509Obj.load_x509_certificate( argv[1]);
    if ( myX509 == nullptr ) {
        std::cout << "Load X509 Certificate Error " << std::endl;
        exit(EXIT_FAILURE);
    }
    X509_print_fp(stdout, myX509);

    return 0;
}
