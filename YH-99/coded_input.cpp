#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

using namespace std;
using namespace google::protobuf::io;

#define BUFFER_SIZE  256

int main(int argc, char* argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if ( argc != 3 ) {
        std::cerr << "Usage:  " << argv[0] << " <Magic Number> <Input FILE>" << std::endl;
        return -1;
    }

    google::protobuf::uint32 magic_number;
    google::protobuf::uint32 strLen;
    google::protobuf::uint32 inNumber = stoi(argv[1]);

    int fd = open(argv[2], O_RDONLY);  // Linux open file system call
    ZeroCopyInputStream* raw_input = new FileInputStream(fd);
    CodedInputStream* coded_input = new CodedInputStream(raw_input);

    coded_input->ReadLittleEndian32(&magic_number);  // First we read a 4-byte "magic number"
    std::cout << "Magic Number : " << magic_number << std::endl;

      if (magic_number != inNumber ) {
        cerr << "File not in expected format." << endl;
        return -1;
    }

    char text[BUFFER_SIZE];
    memset(text, '\0', BUFFER_SIZE);

    coded_input->ReadVarint32(&strLen);  // Read The string length
    coded_input->ReadRaw(text, strLen);  // followed by read the raw bytes with length.
    std::cout << "Client  Name : " << text << " <- " << strLen << std::endl;

    memset(text, '\0', BUFFER_SIZE);
    coded_input->ReadVarint32(&strLen);
    coded_input->ReadRaw(text, strLen);
    std::cout << "Address      : " << text << " <- " << strLen << std::endl;

    memset(text, '\0', BUFFER_SIZE);
    coded_input->ReadVarint32(&strLen);
    coded_input->ReadRaw(text, strLen);
    std::cout << "City State   : " << text << " <- " << strLen << std::endl;

    delete coded_input;
    delete raw_input;
    close(fd);

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
