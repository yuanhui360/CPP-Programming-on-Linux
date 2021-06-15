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
        std::cerr << "Usage:  " << argv[0] << " <Output FILE>" << std::endl;
        return -1;
    }

    Purchase::Order  myOrder;
    myOrder.set_id(10001);
    myOrder.set_vendor("Startbucks Toronto");

    Purchase::orderItem *myItem1 = myOrder.add_item();
    myItem1->set_code(501);
    myItem1->set_desc("Hot Caffè Dark Roast");
    myItem1->set_qty(2);
    myItem1->set_price(2.85);

    Purchase::orderItem *myItem2 = myOrder.add_item();
    myItem2->set_code(502);
    myItem2->set_desc("Hot Cappucino ");
    myItem2->set_qty(1);
    myItem2->set_price(3.45);

    Purchase::orderItem *myItem3 = myOrder.add_item();
    myItem3->set_code(503);
    myItem3->set_desc("Hot Tea ");
    myItem3->set_qty(3);
    myItem3->set_price(2.55);

    std::ofstream output(argv[1], std::ios::out | std::ios::trunc | std::ios::binary);

    /*
     * serializes the message and writes the message to the given C++ ostream.
     */

    if (!myOrder.SerializeToOstream(&output)) {
      std::cerr << "Failed to write message book." << std::endl;
      return -1;
    }

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
