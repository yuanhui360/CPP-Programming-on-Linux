#include <AMQPcpp.h>

//  AMQP amqp("test:test@192.168.0.106:5672/my_vhost");     // all connect string

/*
 *  Fanout Exchange
 *  A fanout exchange routes messages to all of the queues that are bound to it and the
 *  routing key is ignored. If N queues are bound to a fanout exchange, when a new message
 *  is published to that exchange a copy of the message is delivered to all N queues.
 *
 */

using namespace std;

int main (int argc, char** argv) {

    std::string msg_body;

    if (argc != 4 && argc != 3)
    {
        std::cout << "Usage: fanout_publish <host:port> <exchange> [queue]" << std::endl;
        std::cout << "Example :" << std::endl;
        std::cout << "fanout_publish 192.168.0.103:5672  my_exchange my_queue" << std::endl;
        std::cout << "fanout_publish 192.168.0.103:5672  my_exchange" << std::endl;
        return 1;
    }

    /*
     *  [$USERNAME[:$PASSWORD]\@]$HOST[:$PORT]/[$VHOST]
     */

    std::string conn_str = "test:test@";
      conn_str += std::string(argv[1]);
    conn_str += std::string("/my_vhost");
    char const *ex_name = argv[2];

    try {

        AMQP amqp(conn_str);

        AMQPExchange * ex = amqp.createExchange(ex_name);
        ex->Declare(ex_name, "fanout", AMQP_AUTODELETE | AMQP_DURABLE);

        /*
         * short my_param = AMQP_AUTODELETE | AMQP_DURABLE;
         * ex->setParam(my_param);
         */

        if ( argc == 4 ) {
            char const *queue_name = argv[3];
            AMQPQueue * qu2 = amqp.createQueue(queue_name);
            qu2->Declare();
            qu2->Bind( ex_name, "");
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
            ex->Publish( msg_body , "");
            msg_body.clear();
        }


    } catch (AMQPException &ec) {
        std::cout << ec.getMessage() << std::endl;
    }

    return 0;

}
