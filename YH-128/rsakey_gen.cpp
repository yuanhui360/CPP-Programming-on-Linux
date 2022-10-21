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
 *  struct RSA
 *  {
 *      typedef RSAFunction PublicKey;
 *      typedef InvertibleRSAFunction PrivateKey;
 *  };
 */

int main (int argc, char* argv[])
{

    RSA::PrivateKey *p_privateKey;
    RSA::PublicKey  *p_publicKey;

    if (argc != 4) {
        fprintf(stdout, "Usage   : %s <bits> <private key file> <public key file>\n", argv[0]);
        fprintf(stdout, "Example : %s 2048 my_rsa_private_key.pem my_rsa_public_key.pem\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 1) Convert key_bits -> unsigned int
    unsigned int key_bits = (unsigned int) std::atoi( argv[1] );

    // 2) Pseudo Random Number Generator
    AutoSeededRandomPool    rng;

    // 3) Generate Parameters
    InvertibleRSAFunction   params;
    params.GenerateRandomWithKeySize(rng, key_bits);

    // 4) Create Keys
    p_privateKey = new RSA::PrivateKey( params );
    p_publicKey  = new RSA::PublicKey( params );

    // 5) HexEncoder and Save to  private key file, public key file
    HexEncoder   privFile( new FileSink( argv[2] ));
    HexEncoder   pubFile( new FileSink( argv[3] ));
    p_privateKey->Save( privFile );
    p_publicKey->Save( pubFile );

    return 0;
}
