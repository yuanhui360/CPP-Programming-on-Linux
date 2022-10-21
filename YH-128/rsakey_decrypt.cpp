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

#include <iostream>

using namespace CryptoPP;

int main (int argc, char* argv[])
{

    std::string   iv("0123456789012345");   // A 128 bit IV  16 byte
    RandomPool randPool;
    randPool.IncorporateEntropy((unsigned char*)iv.c_str(), iv.size());

    RSA::PrivateKey p_privateKey;
    RSA::PublicKey  p_publicKey;

    // Alice use her own private key to decrypt Bob's encrypted message

    if (argc != 4) {
        fprintf(stdout, "Usage   : %s <private key file> <encrypt file> <decrypted text file>\n", argv[0]);
        fprintf(stdout, "        : %s my_rsa_private.key my_encypt_msg.txt my_decrypt_msg.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Load Private Key from Key file argv[1]
    FileSource   keyFile(argv[1], true, new HexDecoder);
    p_privateKey.Load( keyFile );

    // Instancialize Decryptor Object (RSA OAEP SHA256)
    RSAES<OAEP<SHA256>>::Decryptor     decryptor( p_privateKey );  // <- RSA OAEP, SHA256
    std::cout << "Algorithm Name : " << decryptor.AlgorithmName() << std::endl;
    std::cout << "Fixed Max Plaintext Length : " << decryptor.FixedMaxPlaintextLength() << std::endl;
    std::cout << "Fixed Cipher Text Length : " << decryptor.FixedCiphertextLength() << std::endl;

    // argv[2] -> HexDecoder() -> PK_DecryptorFilter -> argv[3]
    FileSource( argv[2], true, new HexDecoder( new PK_DecryptorFilter(
                randPool, decryptor, new FileSink(argv[3]))));

    return 0;
}
