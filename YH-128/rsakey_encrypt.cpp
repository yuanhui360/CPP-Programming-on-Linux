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
#include <crypto++/seckey.h>
#include <crypto++/pubkey.h>

#include <iostream>

using namespace CryptoPP;

/*
 *  RSAES - RSA Encryption Schemes
 */

int main (int argc, char* argv[])
{

    std::string   iv("0123456789012345"); // A 128 bit IV  16 bytes
    RandomPool randPool;
    randPool.IncorporateEntropy((unsigned char*)iv.c_str(), iv.size());

    RSA::PrivateKey p_privateKey;
    RSA::PublicKey  p_publicKey;

    // Bob use Alice's public key to encrypt his text message

    if (argc != 4) {
        fprintf(stdout, "Usage   : %s <public key file> <text file> <encrypt file>\n", argv[0]);
        fprintf(stdout, "        : %s my_rsa_public.key my_original_msg.txt my_encypt_msg.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Load Public Key from Key file argv[1]
    FileSource   keyFile(argv[1], true, new HexDecoder);
    p_publicKey.Load(keyFile);

    // Instancialize Encryptor Object (RSA OAEP SHA256)
    RSAES<OAEP<SHA256>>::Encryptor    encryptor( p_publicKey );   // <- OAEP SHA256
    std::cout << "Algorithm Name : " << encryptor.AlgorithmName() << std::endl;
    std::cout << "Fixed Max Plaintext Length : " << encryptor.FixedMaxPlaintextLength() << std::endl;
    std::cout << "Fixed Cipher Text Length : " << encryptor.FixedCiphertextLength() << std::endl;

    // argv[2] -> PK_EncryptorFilter -> HexEncoder -> argv[3]
    FileSource( argv[2], true, new PK_EncryptorFilter(randPool, encryptor, new HexEncoder( new FileSink(argv[3]))));

    return 0;
}
