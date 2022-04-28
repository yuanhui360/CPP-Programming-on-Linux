#include "evp_class.h"

#include <cstdlib> //std::system
#include <sstream>

/*
 *  To Verify:
 *
 *    1) Generate a hash of the data file
 *    2) Use the signer’s public key to decrypt the signature file
 *    3) Check that the two hash files match
 *       Obviously the crypto hash algorithm has to be the same in both signing and verification.
 */

int main (int argc, char *argv[])
{

    EVP_PKEY        *my_pkey;

    if (argc != 4) {
        fprintf(stdout, "Usage   : %s <key file> <original file> <signature file>\n", argv[0]);
        fprintf(stdout, "Example : %s my_public_key.pem orignal_file.txt signature_file.dat\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    LibOpenSSL::MyEVP_Key                keyObj;
    my_pkey = keyObj.load_key("public", argv[1]);
    if ( my_pkey == nullptr ) {
        std::cout << "Load Public Key error (" << argv[1] << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
    int rc = keyObj.pkey_verfiy(argv[2], argv[3]);
    if ( rc <= 0 ) {
        std::cout << "verify failed or not match (" << rc << ")" << std::endl;
    }
    else {
        std::cout << "verify success and match (" << rc << ")" << std::endl;
    }

    return 0;
}
