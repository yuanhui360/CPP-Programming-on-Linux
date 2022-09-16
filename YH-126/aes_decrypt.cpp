#include <crypto++/cryptlib.h>

#include <crypto++/base64.h>
#include <crypto++/hex.h>
#include <crypto++/rsa.h>
#include <crypto++/aes.h>
#include <crypto++/files.h>
#include <crypto++/randpool.h>
#include <crypto++/validate.h>
#include <crypto++/modes.h>
#include <crypto++/osrng.h>

#include <iostream>


/*
 *   AES -> Advanced Encryption Standard
 *
 *   Modes of Operation:
 *   ECB Mode -> is electronic codebook.
 *   CBC Mode -> is cipher block chaining.
 *   CTR mode -> is counter mode and uses a counter rather than a random
 *               or unpredictable initialization vector.
 */

int main (int argc, char* argv[])
{
    using namespace CryptoPP;

    unsigned char *key = (unsigned char *)"01234567890123456789012345678901"; // A 256 bit key 32 bytes
    unsigned char *iv =  (unsigned char *)"0123456789012345";                 // A 128 bit IV  16 bytes
    size_t key_size = strlen((const char*)key);

    if (argc != 3) {
        fprintf(stdout, "Usage   : %s <encrypt file> <decrypt file>\n", argv[0]);
        fprintf(stdout, "Example : %s my_encrypt_msg.txt my_decrypt_msg.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     *   1) Instancializing AES Decryptor Object
     *   2) FileSource(argv[1]) -> AES CBC decryptor() -> FileSink(argv[2])
     */

    CBC_Mode<AES>::Decryption        decryptor(key, key_size, iv);
    std::cout << "Algorithm Name : " << decryptor.AlgorithmName() << std::endl;
    std::cout << "Default Key Length : " << decryptor.DefaultKeyLength() << std::endl;
    std::cout << "Key Size : " << key_size << std::endl;
    std::cout << "Maximum Key Length " << decryptor.MaxKeyLength() << std::endl;
    std::cout << "Mimium Key Length : " << decryptor.MinKeyLength() << std::endl;
    std::cout << "IV size : " << decryptor.IVSize() << std::endl;
    std::cout << "Maxmium IV Lenght : " << decryptor.MaxIVLength() << std::endl;
    std::cout << "Minium IV Lenght : " << decryptor.MinIVLength() << std::endl;
    FileSource(argv[1], true, new StreamTransformationFilter(decryptor, new FileSink(argv[2])));
    return 0;
}
