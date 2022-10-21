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

    RSA::PrivateKey p_privateKey;
    RSA::PublicKey  p_publicKey;

    if (argc != 3) {
        fprintf(stdout, "Usage   : %s [private|public] <key file>\n", argv[0]);
        fprintf(stdout, "Example : %s private my_rsa_private.key\n", argv[0]);
        fprintf(stdout, "        : %s public my_rsa_public.key\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FileSource keyFile( argv[2], true, new HexDecoder);
    if ( strcmp( argv[1], "public") == 0 ) {
        /*
         *   Load from key file -> p_publicKey
         *   Parameters
         *      n -> modulus
         *      e -> public exponent
         */

        p_publicKey.Load( keyFile );
        const Integer& n = p_publicKey.GetModulus();
        const Integer& e = p_publicKey.GetPublicExponent();

        std::cout << "RSA Public Key Bits : " << n.BitCount() << std::endl;
        std::cout << "Modulus() n(" << n.BitCount() << ") : " << n << std::endl;
        std::cout << "PublicExponent e(" << e.BitCount() << ") : " << e << std::endl;
        std::cout << "ValueNames() List : " << p_publicKey.GetValueNames() << std::endl;

        std::vector<word32>  oidVec = p_publicKey.GetAlgorithmID().m_values;
        std::cout << "Alogrithm ID (OID) Value : ";
        for ( auto it = oidVec.begin(); it != oidVec.end(); it++ ) {
            std::cout << *it.base() << ".";
        }
        std::cout << std::endl;
    }
    else if ( strcmp( argv[1], "private") == 0 ) {
        /*
         *   Load from key file -> p_privateKey
         *   Parameters
         *      n  -> modulus
         *      e  -> public exponent
         *      d  -> private exponent
         *      p  -> first prime factor
         *      q  -> second prime factor
         *      dp -> d mod p
         *      dq -> d mod q
         *      u  -> q-1 mod p
         */

        p_privateKey.Load( keyFile );
        const Integer& n = p_privateKey.GetModulus();
        const Integer& e = p_privateKey.GetPublicExponent();
        const Integer& d = p_privateKey.GetPrivateExponent();
        const Integer& p = p_privateKey.GetPrime1();
        const Integer& q = p_privateKey.GetPrime2();
        const Integer& dp = p_privateKey.GetModPrime1PrivateExponent();
        const Integer& dq = p_privateKey.GetModPrime2PrivateExponent();
        const Integer& u = p_privateKey.GetMultiplicativeInverseOfPrime2ModPrime1();

        std::cout << "RSA Private Key Bits : " << n.BitCount() << std::endl;
        std::cout << "Modulus n(" << n.BitCount() << ") : " << n << std::endl;
        std::cout << "PublicExponent e(" << e.BitCount() << ") : " << e << std::endl;
        std::cout << "PrivateExponent d(" << d.BitCount() << ") : " << d << std::endl;
        std::cout << "Prime1 p(" << p.BitCount() << ") : " << p << std::endl;
        std::cout << "Prime2 q(" << q.BitCount() << ") : " << q << std::endl;
        std::cout << "ModPrime1PrivateExponent dp(" << dp.BitCount() << ") : " << dp << std::endl;
        std::cout << "ModPrime2PrivateExponent dq(" << dq.BitCount() << ") : " << dq << std::endl;
        std::cout << "MultiplicativeInverseOfPrime2ModPrime1 u(" << u.BitCount() << ") : " << u << std::endl;

        std::cout << "ValueNames() List : " << p_privateKey.GetValueNames() << std::endl;

        std::vector<word32>  oidVec = p_privateKey.GetAlgorithmID().m_values;
        std::cout << "Alogrithm ID (OID) Value : ";
        for ( auto it = oidVec.begin(); it != oidVec.end(); it++ ) {
            std::cout << *it.base() << ".";
        }
        std::cout << std::endl;

    }
    else {
        std::cerr << "Argument argv[1] must be [private|public]" << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}   
