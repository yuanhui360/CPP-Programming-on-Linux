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

    if (argc != 4) {
        // Alice use her private key to decrypt message
        fprintf(stdout, "Usage   : %s <private key file> <encrypt file> <decrypt file>\n", argv[0]);
        fprintf(stdout, "        : %s my_rsa_private.key my_encypt_msg.txt my_decrypt_msg.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Load Private Key from Key file
    FileSource keyFile(argv[1], true, new HexDecoder);
    p_privateKey.Load(keyFile);

    // Instancialize Decryption Object
    RSAES<OAEP<SHA256>>::Decryptor  decryptor(p_privateKey);

    // Get Maxmium Plaintext and Cipher Length
    size_t text_length =  decryptor.FixedMaxPlaintextLength();
    size_t cipher_length =  decryptor.FixedCiphertextLength();
    std::cout << "Max Text Length : " << text_length << " Max Cipher Length : " << cipher_length << std::endl;

    // Loop {argv[2] -> cipher_str -> PK_DecryptorFilter -> argv[3]}
    std::string     cipher_str;
    FileSource      encFile( argv[2], false, new HexDecoder( new StringSink(cipher_str)));
    FileSink        textFile( argv[3], false );

    while ( encFile.Pump( cipher_length * 2) )
    {
        std::string  plaintext_str;
        StringSource( cipher_str, true, new PK_DecryptorFilter(randPool, decryptor, new StringSink(plaintext_str)));
        textFile.Put( (unsigned char*)plaintext_str.c_str(), plaintext_str.size(), true);
        cipher_str.clear();
    }
    textFile.MessageEnd();


    return 0;
}
