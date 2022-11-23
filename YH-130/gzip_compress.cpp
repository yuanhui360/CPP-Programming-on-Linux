#include <crypto++/cryptlib.h>

#include <crypto++/base64.h>
#include <crypto++/hex.h>
#include <crypto++/files.h>
#include <crypto++/gzip.h>

#include <iostream>

using namespace CryptoPP;

int main (int argc, char* argv[])
{

    if (argc != 3) {
        fprintf(stdout, "Usage   : %s <original file> <gzip file>\n", argv[0]);
        fprintf(stdout, "        : %s my_original_msg.txt my_gzip_msg.gz\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     *  Pipelining :
     *  FileSource(argv[1]) -> Gzip() -> FileSink(argv[2])
     */

    FileSource( argv[1], true, new Gzip( new FileSink( argv[2] )));
    return 0;
}
