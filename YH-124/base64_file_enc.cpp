#include <crypto++/cryptlib.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <crypto++/base64.h>
#include <crypto++/files.h>

#include <iostream>

/*
 *  Base64Encoder -> The Base64Encoder encodes bytes into base 64 characters.
 *  Base64Decoder -> The Base64Decoder converts base 64 encoded data into binary data.
 */

int main (int argc, char* argv[])
{
    using namespace CryptoPP;

    if (argc != 3) {
        fprintf(stdout, "Usage   : %s <Source File> <Encrypt File>\n", argv[0]);
        fprintf(stdout, "Example : %s source_message.txt dest_message.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     *  FileSource(argv[1]) -> Base64Encoder() -> FileSink(argv[2])
     */

    FileSource  file_source(argv[1], true, new Base64Encoder(new FileSink(argv[2])));

    return 0;
}
