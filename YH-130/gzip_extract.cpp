#include <crypto++/cryptlib.h>

#include <crypto++/base64.h>
#include <crypto++/files.h>
#include <crypto++/gzip.h>

#include <iostream>

using namespace CryptoPP;

int main (int argc, char* argv[])
{

    if (argc != 3) {
        fprintf(stdout, "Usage   : %s <gzip file> <extracted file>\n", argv[0]);
        fprintf(stdout, "        : %s my_gzip_msg.gz my_extract_file.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     * FileSource(argv[1]) -> Gunzip() -> FileSink(argv[2])
     */

    FileSource( argv[1], true, new Gunzip( new FileSink( argv[2] )));
    return 0;
}
