#include <iostream>
#include <crypto++/cryptlib.h>
#include <crypto++/files.h>

/*
 *  Advantage Crypto++ is it's Pipelining design pattern that works in a way
 *  similar to the Unix/Linux shell pipes.
 *  Example :
 *     Source -> Filter -> ... -> Filter -> Sink
 *
 *  In the Pipelining paradigm:
 *     Source -> Sources are the origin of data.
 *               For Example :
 *                 FileSource(), StringSource(), SocketSource()
 *     Sink   -> Sink is the destination of transformed data.
 *               For Example:
 *                 FileSink(), StringSink(), SocketSink()
 *     Filter -> Filter is an object which transform data.
 *               For Example:
 *               base64Encoder(), Base64Decoder(), base16Encoder(), Base16Decoder()
 *
 */

int main (int argc, char* argv[])
{
    using namespace CryptoPP;

    if (argc != 2) {
        fprintf(stdout, "Usage   : %s <Source File>\n", argv[0]);
        fprintf(stdout, "Example : %s source_message.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  
    /*
     *  File (FileSource) -> String (StringSink) -> std::cout
     *
     *  Constructor
     *  FileSource (const char *filename,
     *              bool pumpAll,
     *              BufferedTransformation *attachment=NULL,
     *              bool binary=true)
     *
     *  filename    : is the name of a file. The FileSource will open the file as an istream.
     *  pumpAll     : specifies whether all the data should be red and processed.
     *  attachment  : is a BufferedTransformation, such as another filter or sink.
     *  binary      : is passed to the underlying ostream (attachment)
     */

     std::string      dest_str;
     FileSource       my_file_source(argv[1], true, new StringSink(dest_str));
     std::cout << dest_str;

    return 0;
}
 
