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

    // Bob use his private key to sign document file

    if (argc != 4) {
        fprintf(stdout, "Usage   : %s <private key file> <original file> <signature text file>\n", argv[0]);
        fprintf(stdout, "        : %s my_rsa_private.key my_original_msg.txt my_signature.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Load Private Key from Key file argv[1]
    FileSource   keyFile(argv[1], true, new HexDecoder);
    p_privateKey.Load( keyFile );

    // Instancialize Signer Object (RSA OAEP SHA256)
    RSASS<PKCS1v15, SHA256>::Signer           signer(p_privateKey);
    std::cout << "Algorithm Name : " << signer.AlgorithmName() << std::endl;
    std::cout << "Max Signature Length : " << signer.MaxSignatureLength() << std::endl;

    // argv[2] -> SignertorFilter -> HexEncoder -> argv[3]
    FileSource(argv[2], true, new SignerFilter(randPool, signer, new HexEncoder(new FileSink(argv[3]))));

    return 0;
}
