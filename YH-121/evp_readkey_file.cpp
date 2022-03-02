#include "evp_class.h"

int main(int argc, char* argv[]) {

    bool         is_private = true;
    EVP_PKEY     *my_pkey = nullptr;

    if (argc != 3) {
        fprintf(stdout, "Usage   : %s [public|private] <key file>\n", argv[0]);
        fprintf(stdout, "Example : %s public my_public_key.pem\n", argv[0]);
        fprintf(stdout, "          %s private my_private_key.pem\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    LibOpenSSL::MyEVP_Key        keyObj;
    my_pkey = keyObj.load_key(argv[1], argv[2]);
    if ( my_pkey == nullptr ) {
        std::cout << "Read Key File Error " << std::endl;
        exit(EXIT_FAILURE);
    }

    if ( strcmp( argv[1], "public") == 0 ) {
        is_private = false;
    }
    else if ( strcmp( argv[1], "private") == 0 ) {
        is_private = true;
    }

    keyObj.display_key( is_private );
    std::cout << "------------------------------" << std::endl;
    std::cout << "Key Type : " << argv[1] << " (" << keyObj.get_keytype() << ")" << std::endl;
    std::cout << "EVP+PKEY size : " << keyObj.get_pkey_size() << " ("
              << keyObj.get_pkey_bits() << " bits)" << std::endl;

    return 0;
}
