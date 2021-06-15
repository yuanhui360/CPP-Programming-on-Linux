#include <iostream>
#include <string>
#include <fstream>
#include <google/protobuf/arena.h>
#include <google/protobuf/message.h>
#include "Order.pb.h"

using namespace google::protobuf;

int main(int argc, char* argv[])
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if ( argc != 2 ) {
        std::cerr << "Usage:  " << argv[0] << " <Input FILE>" << std::endl;
        return -1;
    }

    std::ifstream input(argv[1], std::ios::in | std::ios::binary);
    Purchase::Order myOrder;
    if ( !myOrder.ParseFromIstream(&input) ) {
        std::cerr << "Failed to parse message data." << std::endl;
        return -1;
    }

    std::cout << "Order    id : " << myOrder.id() << std::endl;
    std::cout << "vendor Name : " << myOrder.vendor() << std::endl;
    std::cout << "Order Items : " << std::endl;
    for ( int i = 0; i < myOrder.item_size(); i++ ) {
        std::cout << myOrder.item(i).code() << " : "
                  << myOrder.item(i).desc() << " : "
                  << myOrder.item(i).qty() << " : "
                  << myOrder.item(i).price() << std::endl;
    }

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
