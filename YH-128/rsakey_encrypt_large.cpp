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

/*
 *
 *  void RandomPool::IncorporateEntropy(const byte* input, size_t length )
 *  Update RNG state with additional unpredictable values.
 *
 *  Parameters
 *      input  : the entropy to add to the generator
 *      length : the size of the input buffer
 *
 */

int main (int argc, char* argv[])
{

    std::string   iv("0123456789012345");   // A 128 bit IV  16 byte
    RandomPool randPool;
    randPool.IncorporateEntropy((unsigned char*)iv.c_str(), iv.size());

    RSA::PrivateKey p_privateKey;
    RSA::PublicKey  p_publicKey;

    if (argc != 4) {
        // Bob use Alice's public key to encrypt message
        fprintf(stdout, "Usage   : %s <public key file> <text file> <encrypt file>\n", argv[0]);
        fprintf(stdout, "        : %s my_rsa_public.key my_text_msg.txt my_encypt_msg.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Load Public Key from Key file
    FileSource keyFile(argv[1], true, new HexDecoder);
    p_publicKey.Load(keyFile);

    // Instancialize Encryption object
    RSAES<OAEP<SHA256>>::Encryptor   encryptor( p_publicKey );    // <- OAEP. SHA256

    // Get Maxmium Plaintext and Cipher Length
    size_t text_length =  encryptor.FixedMaxPlaintextLength();
    size_t cipher_length =  encryptor.FixedCiphertextLength();
    std::cout << "Max Text Length : " << text_length << " Max Cipher Length : " << cipher_length << std::endl;

    // Loop {argv[2] -> text_str -> PK_EncryptorFilter -> HexEncoder -> argv[3]}
    std::string       text_str;
    FileSource  textFile( argv[2], false, new StringSink( text_str ));
    HexEncoder  encFile( new FileSink( argv[3], false ));

    while ( textFile.Pump( text_length ) )
    {
        std::string      cipher_str;
        StringSource( text_str, true, new PK_EncryptorFilter(randPool, encryptor, new StringSink(cipher_str)));
        encFile.Put((unsigned char*)cipher_str.c_str(), cipher_str.size(), true);
        text_str.clear();
    }
    encFile.MessageEnd();

    return 0;
}
