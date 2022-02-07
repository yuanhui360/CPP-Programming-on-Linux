#include "evp_class.h"

int main(int argc, char* argv[]) {

    EVP_PKEY         *myPKey = NULL;
    unsigned int     myKeyBits;

    if (argc != 4) {
        fprintf(stdout, "Usage   : %s <bits> <private key_file> <public key_file>\n", argv[0]);
        fprintf(stdout, "Example : %s 2048 my_rsa_private_key.pem my_rsa_public_key.pem\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    myKeyBits = (unsigned int)atoi(argv[1]);
    LibOpenSSL::MyEVP_Key        keyObj;
    myPKey = keyObj.generate_rsa_key(myKeyBits);
    if ( myPKey == NULL ) {
        std::cout << "generate RSA Key error" << std::endl;
        exit(EXIT_FAILURE);
    }

    keyObj.write_key(argv[2], true);     // <- Private key file
    keyObj.write_key(argv[3], false);    // <- Public key file

    return 0;
}
