#include <crypto++/cryptlib.h>

#include <crypto++/sha.h>

#include <iostream>

void print_hash(unsigned char *value, size_t length) {

     printf("Hash Value : (%d) ", (int)length);
     for (size_t i = 0; i < length; i++)
         printf("%02x:", value[i]);
     printf("\n");
}

int main (int argc, char* argv[])
{
    using namespace CryptoPP;

    std::string digest;
    int msg_size;

    if (argc != 2) {
        fprintf(stdout, "Usage   : %s <message>\n", argv[0]);
        fprintf(stdout, "Example : %s \"This is Hui Test\"\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     *  1) Get message from command line argv[1]
     *  2) Using SHA256 algorithm digest message
     *  3) Print out both original message and digested message
     */

    msg_size = strlen( argv[1] );
    SHA256            hash;
    std::cout << "Algorithm   : " << hash.AlgorithmName() << std::endl;
    std::cout << "Digest size : " << hash.DIGESTSIZE << std::endl;
    std::cout << "Block  size : " << hash.BlockSize() << std::endl;
    std::cout << "Tag    size : " << hash.TagSize() << std::endl;

    hash.Update( (const byte*)argv[1], msg_size);
    digest.resize(hash.DigestSize());
    hash.Final( (byte*)&digest[0]);

    std::cout << "-------------------------" << std::endl;
    std::cout << "Message : (" << msg_size << ") " << argv[1] << std::endl;
    print_hash( (byte*) digest.c_str(), hash.DigestSize());

    return 0;
}

