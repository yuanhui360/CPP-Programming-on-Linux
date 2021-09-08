#include <AMQPcpp.h>

//  AMQP amqp("test:test@192.168.0.106:5672/my_vhost");     // all connect string

/*
 *
 *  Topic Exchange
 *  Topic exchanges route messages to one or many queues based on matching between a
 *  message routing key and the pattern that was used to bind a queue to an exchange.
 *  The topic exchange type is often used to implement various publish/subscribe pattern
 *  variations. Topic exchanges are commonly used for the multicast routing of messages.
 *
 *  1) The routing key must be a list of words, delimited by a period (.).
 *  2) The routing patterns may contain an asterisk (“*”) to match a word in a specific
 *     nposition of the routing key
 *     (e.g., a routing pattern of "agreements.*.*.b.*" only match routing keys where the
 *      first word is "agreements" and the fourth word is "b").
 *
 */

using namespace std;

int main (int argc, char** argv) {

    std::string msg_body;

    if (argc != 5 && argc != 4)
    {
        std::cout << "Usage: topic_publish <host:port> <exchange> <topic pattern> [queue]" << std::endl;
        std::cout << "Example :" << std::endl;
        std::cout << "topic_publish 192.168.0.103:5672 key1_exchange Order.*.* top_queue" << std::endl;
        std::cout << "topic_publish 192.168.0.103:5672 key1_exchange Order.*.*" << std::endl;
        return 1;
    }

    /*
     *  [$USERNAME[:$PASSWORD]@]$HOST[:$PORT]/[$VHOST]
     */

    std::string conn_str = "test:test@";
    conn_str += std::string(argv[1]);
    conn_str += std::string("/my_vhost");
    char const *ex_name = argv[2];
    char const *routing_key = argv[3];

    try {

        AMQP amqp(conn_str);

        AMQPExchange * ex = amqp.createExchange(ex_name);
        ex->Declare(ex_name, "topic");

        short my_param = AMQP_AUTODELETE | AMQP_DURABLE;
        ex->setParam(my_param);

        ex->setHeader("Delivery-mode", AMQP_DELIVERY_PERSISTENT);
        ex->setHeader("Content-type", "text/text");
        ex->setHeader("Content-encoding", "UTF-8");

        if ( argc == 5 ) {
            char const *queue_name = argv[4];
            AMQPQueue * qu2 = amqp.createQueue(queue_name);
            qu2->Declare();
            qu2->Bind( ex_name, routing_key);
        }

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
