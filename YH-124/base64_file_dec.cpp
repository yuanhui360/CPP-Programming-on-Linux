#include <crypto++/cryptlib.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <crypto++/base64.h>
#include <crypto++/files.h>

#include <iostream>

int main (int argc, char* argv[])
{
    using namespace CryptoPP;

    if (argc != 3) {
        fprintf(stdout, "Usage   : %s <Encorded File> <Decorded File>\n", argv[0]);
        fprintf(stdout, "Example : %s encrypt_msg.dat decrypt_msg.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     *  FileSource(argv[1]) -> Base64Decoder() -> FileSink(argv[2])
     */

    FileSource file_source(argv[1], true, new Base64Decoder(new FileSink(argv[2])));
    return 0;
}
