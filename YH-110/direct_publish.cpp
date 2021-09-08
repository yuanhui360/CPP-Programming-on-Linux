#include <AMQPcpp.h>

//  AMQP amqp("test:test@192.168.0.106:5672/my_vhost");     // all connect string

/*
 *  Direct Exchange
 *  A direct exchange delivers messages to queues based on the message routing key.
 *  Here is how it works:
 *
 *   1. A queue binds to the exchange with a routing key K
 *   2. When a new message with routing key R arrives at the direct exchange, the exchange
 *      routes it to the queue if K = R
 *
 */

using namespace std;

int main (int argc, char** argv) {

    std::string msg_body;

    if (argc != 5 && argc != 4)
    {
        std::cout << "Usage: direct_publish <host:port> <exchange> <rounting key> [queue]" << std::endl;
        std::cout << "Example :" << std::endl;
        std::cout << "direct_publish 192.168.0.103:5672 dirExchange key1 direct_queue" << std::endl;
        std::cout << "direct_publish 192.168.0.103:5672 dirExchange key1" << std::endl;
        return 1;
    }

    /*
     *  [$USERNAME[:$PASSWORD]@]$HOST[:$PORT]/[$VHOST]
     *  "test:test@192.168.0.106:5672/my_vhost"
     */

    std::string conn_str = "test:test@";
    conn_str += std::string(argv[1]);
    conn_str += std::string("/my_vhost");
    char const *ex_name = argv[2];
    char const *routing_key = argv[3];

    try {

        AMQP amqp(conn_str);

        AMQPExchange * ex = amqp.createExchange(ex_name);
        ex->Declare(ex_name, "direct");

        short my_param = AMQP_AUTODELETE | AMQP_DURABLE;
        ex->setParam(my_param);

        if ( argc == 5 ) {
            char const *queue_name = argv[4];
            AMQPQueue * qu2 = amqp.createQueue(queue_name);
            qu2->Declare();
            qu2->Bind( ex_name, routing_key);
        }

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
            ex->Publish( msg_body , routing_key);
            msg_body.clear();
        }


    } catch (AMQPException &ec) {
        std::cout << ec.getMessage() << std::endl;
    }

    return 0;

}
