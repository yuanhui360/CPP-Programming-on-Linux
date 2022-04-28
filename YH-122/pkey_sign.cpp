#include "evp_class.h"

#include <cstdlib> //std::system
#include <sstream>

/*
 * To Sign
 *   1)  Generate a hash of the data file
 *   2)  Encrypt the hash with a private key producing a signature file
 *   3)  Distribute the data and signature files
 */

int main (int argc, char *argv[])
{

    EVP_PKEY        *my_pkey;

    if (argc != 4) {
        fprintf(stdout, "Usage   : %s <key file> <original file> <signature file>\n", argv[0]);
        fprintf(stdout, "Example : %s my_private_key.pem orignal_file.txt signature_file.dat\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    LibOpenSSL::MyEVP_Key                  keyObj;
    my_pkey = keyObj.load_key("private", argv[1]);
    if ( my_pkey == nullptr ) {
        std::cout << "Load Key file error (" << argv[1] << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
    int rc = keyObj.pkey_sign(argv[2], argv[3]);
    if ( rc <= 0 ) {
        std::cout << "generate signature failed (" << rc << ")" << std::endl;
    }
    else {
        std::cout << "generate signature success (" << rc << ")" << std::endl;
    }

    return 0;
}
