#include <AMQPcpp.h>
#include <libssh/libssh.h>
#include <libssh/libsshpp.hpp>

using namespace std;

int main (int argc, char** argv) {

    std::string msg_body;

    if (argc != 3)
    {
        std::cout << "Usage: my_publish <exchange> <queue>" << std::endl;
        return 1;
    }

    char const *ex_name = argv[1];
    char const *queue_name = argv[2];

    try {

        //  amqp://[$USERNAME[:$PASSWORD]\@]$HOST[:$PORT]/[$VHOST]
        AMQP amqp("test:test@localhost:5672/my_vhost");     // all connect string

        AMQPExchange * ex = amqp.createExchange(ex_name);
        ex->Declare(ex_name, "fanout");

        AMQPQueue * qu2 = amqp.createQueue(queue_name);
        qu2->Declare();
        qu2->Bind( ex_name, "");

        ex->setHeader("Delivery-mode", AMQP_DELIVERY_PERSISTENT);
        ex->setHeader("Content-type", "text/text");
        ex->setHeader("Content-encoding", "UTF-8");

        /*
         * void Publish( std::string message, std::string key )
         */

        for (;;) {
            std::cout << "Please input Message : ";
            std::getline(std::cin, msg_body);
            if ( msg_body == "end" ) {
                break;
            }
            ex->Publish( msg_body , "");
            msg_body.clear();
        }

    } catch (AMQPException &ec) {
        std::cout << ec.getMessage() << std::endl;
    }

    return 0;

}
