#include <iostream>
#include <stdio.h>
#include <string.h>

#include <openssl/bio.h>
#include <openssl/evp.h>

/*
 *  OpenSSL::Digest
 *    A Message Digest or Hash Function takes any arbitrary message
 *    (with any content or length) as an input and provides a fixed size
 *    hash value as a result
 *
 *    1) Message Digest implements a secure one-way function.
 *       Original Data -> Secure Hash Value
 *    2) From Hash Value there is no indication about the
 *       original data that produced it.
 *    3) The only way to identify the original data is to “brute-force”
 *       through every  possible combination of inputs.
 *    4) Every message digest algorithm has a fixed-length output
 *       Example:
 *       SHA256  : Secure Hash Algorithm 256 bits hash value 32 Bytes
 *       MD5     : The MD5 message-digest algorithm 128-bits hash value 16 bytes.
 *       SHA512  : Secure Hash Algorithm 512 bits hash value 64 Bytes (Longest Known)
 *    5) Two distinct inputs the probability that both yield the same output is
 *       highly unlikely.
 *
 *  Usage Example :
 *    1) create unique identifiers for arbitrary data or binary documents.
 *    2) ideal for public key signature algorithms
 *    3) To validate the integrity of a signed document
 *    4) Digital signatures or Digital certificates
 *
 *  Supported digest
 *  SHA, SHA1, SHA224, SHA256, SHA384 and SHA512
 *  MD2, MD4, MDC2 and MD5
 *
 *
 */

int main(int argc, char *argv[])
{
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;

    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned char plaintext[EVP_MAX_MD_SIZE];
    unsigned int md_len;

    memset(md_value, '\0', EVP_MAX_MD_SIZE);
    memset(plaintext, '\0', EVP_MAX_MD_SIZE);

    if (argc != 3) {
        printf("Usage: evp_digest_disp <algorithm> <messgae>\n");
        printf("Example : evp_digest_disp SHA256 \"Hello World\"\n");
        printf("Supported Digets Name :\n");
        printf("   SHA, SHA1, SHA224, SHA256, SHA384 and SHA512 \n");
        printf("   MD2, MD4, MDC2 and MD5 \n");
        exit(1);
    }

    md = EVP_get_digestbyname(argv[1]);
    if (md == NULL) {
        printf("Unknown message digest %s\n", argv[1]);
         exit(1);
    }

    int text_len = strlen(argv[2]);
    memcpy(plaintext, argv[2], strlen(argv[2]));

    /*
     *  EVP Message Digest Process Steps
     *    1) Create a Message Digest context (Allocate Memory)
     *       EVP_MD_CTX_new()
     *    2) Initialise the context by identifying the algorithm (evp.h)
     *       EVP_DigestInit_ex()
     *    3) Digest (Hashes) N bytes of data into the digest context ctx.
     *       EVP_DigestUpdate()  <-- Can be called several times
     *    4) Finalize Retrieves the digest result value from ctx and places
     *       it into  output buffer.
     *       EVP_DigestFinal_ex()
     *    5) Cleans up digest context ctx and frees up the space allocated to it.
     *       EVP_MD_CTX_free()
     *    6) Print out MD buffer on console (stdout)
     */


    /*
     *  EVP_MD_CTX_new()
     *  Allocates and returns a digest context.
     */

    mdctx = EVP_MD_CTX_new();

    /*
     *  EVP_DigestInit_ex()
      *  Sets up digest context ctx to use a digest type from ENGINE impl. type will typically
     *  be supplied by a function such as EVP_sha1(). If impl is NULL then the default
     *  implementation of digest type is used.
     */

    EVP_DigestInit_ex(mdctx, md, NULL);

    /*
     *  EVP_DigestUpdate()
     *  Hashes cnt bytes of data at d into the digest context ctx. This function can be called
     *  several times on the same ctx to hash additional data.
     */

    EVP_DigestUpdate(mdctx, plaintext, text_len);

    /*
     *  EVP_DigestFinal_ex()
     *  Retrieves the digest value from ctx and places it in md. If the s parameter is not NULL
     *  then the number of bytes of data written (i.e. the length of the digest) will be written
     *  to the integer at s, at most EVP_MAX_MD_SIZE bytes will be written. After calling
     *  EVP_DigestFinal_ex() no additional calls to EVP_DigestUpdate() can be made,
     *  but EVP_DigestInit_ex() can be called to initialize a new digest operation.
     */

    EVP_DigestFinal_ex(mdctx, md_value, &md_len);

    /*
     *  EVP_MD_CTX_free()
     *  Cleans up digest context ctx and frees up the space allocated to it.
     *  If digest contexts are not cleaned up after use, memory leaks will occur.
     */

    EVP_MD_CTX_free(mdctx);

    BIO_dump_fp(stdout, (const char*)md_value, md_len);
    std::cout << "length of " << argv[1] << " is : " << md_len << " Bytes\n";
    exit(0);
}
