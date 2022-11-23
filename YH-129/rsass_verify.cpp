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

    // Alice use Bob's public key to verify original document against signature file

    if (argc != 4) {
        fprintf(stdout, "Usage   : %s <public key file> <original file> <signature file>\n", argv[0]);
        fprintf(stdout, "        : %s my_rsa_public.key my_original_msg.txt my_signature.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Load Public Key from Key file argv[1]
    FileSource   keyFile(argv[1], true, new HexDecoder);
    p_publicKey.Load(keyFile);

    // Instancialize Encryptor Object (RSA OAEP SHA256)
    RSASS<PKCS1v15, SHA256>::Verifier          verifier(p_publicKey);

    // concatenation of signature+message <- DEFAULT_FLAG = SIGNATURE_AT_BEGIN
    // (argv[3] + argv[2] ) -> SignatureVerificationFilter -> result(good or bad)
    byte result = 0;
    SignatureVerificationFilter     filter(verifier, new ArraySink(&result, sizeof(result)));
    FileSource  msgFile(argv[2], true);
    FileSource  signFile(argv[3], true, new HexDecoder);

    signFile.TransferTo( filter );
    msgFile.TransferTo( filter );
    filter.MessageEnd();

    // Verify result if verification is OK or not
    std::cout << "********************************" << std::endl;
    if ( result )
        std::cout << "Successfully Verified Signature on File " << std::endl;
    else
        std::cout << "Failed to Verify Signature on File " << std::endl;


    return 0;
}
