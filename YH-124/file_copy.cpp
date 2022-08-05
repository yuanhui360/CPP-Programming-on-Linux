#include <iostream>
#include <crypto++/cryptlib.h>
#include <crypto++/files.h>

int main (int argc, char* argv[])
{
    using namespace CryptoPP;

    if (argc != 3) {
        fprintf(stdout, "Usage   : %s <Source File> <Dest File>\n", argv[0]);
        fprintf(stdout, "Example : %s source_message.txt dest_message.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     *  File (FileSource) -> File (FileSink)
     */

     FileSource   my_file_source(argv[1], true, new FileSink(argv[2]));

    return 0;
}
