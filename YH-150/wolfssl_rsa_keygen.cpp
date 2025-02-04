#include <stdio.h>
#include <string>

#ifndef WOLFSSL_KEY_GEN
#define WOLFSSL_KEY_GEN
#endif

#ifndef WOLFSSL_CERT_GEN
#define WOLFSSL_CERT_GEN
#endif

#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/rsa.h>
#include <wolfssl/wolfcrypt/asn_public.h>
#include <wolfssl/wolfcrypt/asn.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include <wolfssl/wolfcrypt/wc_port.h>

int main(int argc, char** argv)
{
    int ret = 0;

    if ( argc != 4 ) {
        printf("usage: %s <key_size> <private key> <public key>\n", argv[0]);
        printf("Example : %s 2048 my_private_key.der my_public_key.der\n", argv[0]);
        ret = -1;
        return ret;
    }

    int rsaKeySz = std::stoi(argv[1]);
    printf("Creating the RSA private key %s size %d\n", argv[2], rsaKeySz);

    ret = wolfCrypt_Init();

    WC_RNG  rng;
    wc_InitRng( &rng );        // Gets the seed (from OS) and key cipher for rng
    RsaKey  newKey;
    ret = wc_InitRsaKey_ex( &newKey, NULL, INVALID_DEVID);  // Initialize provided RsaKey Struct
    byte *keyBuf = (byte*)XMALLOC(RSA_MAX_SIZE, NULL, DYNAMIC_TYPE_KEY);  // Allocate Memory
    XMEMSET(keyBuf, '\0', RSA_MAX_SIZE);

    ret = wc_MakeRsaKey( &newKey, rsaKeySz, WC_RSA_EXPONENT, &rng); // <-- Generate Private Key
    int keyBufSz = wc_RsaKeyToDer( &newKey, keyBuf, rsaKeySz);      // <-- Convert RsaKey to DER format
    if ( keyBufSz < 0 ) {
        // Error Handling
    }

    FILE   *file_priv = fopen(argv[2], "wb");     // <-- Create Private Key File
    ret = (int)fwrite(keyBuf, 1, keyBufSz, file_priv);
    fclose(file_priv);
    printf("Successfully Generate RSA Private Key %s size %d\n", argv[2], rsaKeySz);

    XMEMSET(keyBuf, '\0', RSA_MAX_SIZE);
    keyBufSz = wc_RsaKeyToPublicDer_ex(&newKey, keyBuf, rsaKeySz, 0);  // Convert RSA Public key to DER f
    // <-- Create Public Key File

    FILE   *file_pub = fopen(argv[3], "wb");     // <-- Create Public Key File
    ret = (int)fwrite(keyBuf, 1, keyBufSz, file_pub);
    fclose(file_pub);
    printf("Successfully Generate RSA Public Key %s size %d\n", argv[3], rsaKeySz);

    XFREE(keyBuf, NULL, DYNAMIC_TYPE_KEY);
    ret = wc_FreeRsaKey(&newKey);
    ret =  wc_FreeRng(&rng);
    ret = wolfCrypt_Cleanup();
    return ret;
}
