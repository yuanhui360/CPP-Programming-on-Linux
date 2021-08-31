#include <AMQPcpp.h>

//  amqp://[$USERNAME[:$PASSWORD]\@]$HOST[:$PORT]/[$VHOST]
//  AMQP amqp("test:test@192.168.0.106:5672/my_vhost");     // all connect string

using namespace std;

int main (int argc, char** argv) {

    std::string msg_body;

    if (argc != 4)
    {
        std::cout << "Usage: my_publish <host:port> <exchange> <queue>" << std::endl;
        std::cout << "Example :" << std::endl;
        std::cout << "my_publish \"192.168.0.106:5672\"  my_exchange my_queue" << std::endl;
        return 1;
    }

    std::string conn_str = "test:test@";
    conn_str += std::string(argv[1]);
    conn_str += std::string("/my_vhost");
    char const *ex_name = argv[2];
    char const *queue_name = argv[3];

    try {

        AMQP amqp(conn_str);

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

