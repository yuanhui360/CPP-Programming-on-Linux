#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#define PERMS 0644
#define BUFFER_SIZE  256

using namespace std;
using namespace google::protobuf::io;

int main(int argc, char* argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    char Name[BUFFER_SIZE]    = "Claudio Fugazzola";
    char Address[BUFFER_SIZE] = "2201 Avenue of The Americans";
    char State[BUFFER_SIZE]   = "New York, NY, 10030, USA";

    if ( argc != 3 ) {
        std::cerr << "Usage:  " << argv[0] << " <Magic_Number> <Output FILE>" << std::endl;
        return -1;
    }

    google::protobuf::uint32 strLen;
    int fd = open(argv[2], O_CREAT | O_WRONLY, PERMS);  // Linux open file system call
    ZeroCopyOutputStream* raw_output = new FileOutputStream(fd);
    CodedOutputStream* coded_output = new CodedOutputStream(raw_output);

    /*
     * Step of using Coded Stream class to  Write data to file.
     *   1) First we write a 4-byte "magic number" to identify the file type,
     *   2) then write a length-delimited string.
     *
     * The string is composed of a varint giving the length followed by the raw bytes.
     */

    google::protobuf::uint32 magic_number = stoi(argv[1]);
    std::cout << "Magic Number : " << magic_number << std::endl;
    coded_output->WriteLittleEndian32(magic_number); // First we write a 4-byte "magic number"

    strLen = strlen(Name);
    std::cout << "Client  Name : " << Name << " <- " << strLen << std::endl;
    coded_output->WriteVarint32(strLen);    // The string is composed of a varint giving the length
    coded_output->WriteRaw(Name, strLen);   // followed by the raw bytes.

    strLen = strlen(Address);
    std::cout << "Address      : " << Address << " <- " << strLen << std::endl;
    coded_output->WriteVarint32(strLen);
    coded_output->WriteRaw(Address, strLen);

    strLen = strlen(State);
    std::cout << "City State   : " << State << " <- " << strLen << std::endl;
    coded_output->WriteVarint32(strLen);
    coded_output->WriteRaw(State, strLen);

    delete coded_output;
    delete raw_output;
    close(fd);  // Linux close file system call

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
