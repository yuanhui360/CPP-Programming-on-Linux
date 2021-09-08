#include <AMQPcpp.h>
#include <string>

using namespace std;

int main (int argc, char* argv[]) {

    if (argc != 3 && argc != 4)
    {
        std::cout << "Usage: read_queue <host:port> <queue> [--include_header]" << std::endl;
        std::cout << "Example :" << std::endl;
        std::cout << "read_queue 192.168.0.103:5672  my_queue" << std::endl;
        std::cout << "read_queue 192.168.0.103:5672  my_queue --include_header" << std::endl;
        return 1;
    }

    std::string conn_str = "test:test@";
    conn_str += std::string(argv[1]);
    conn_str += std::string("/my_vhost");
    char const *queue_name = argv[2];
    bool include_hdr = false;
    if ( argc == 4 ) {
        if ( !strcmp(argv[3], "--include_header" ) ) {
            include_hdr = true;
        }
    }

    try {

        /*
         *  amqp://[$USERNAME[:$PASSWORD]\@]$HOST[:$PORT]/[$VHOST]
         *  AMQP amqp("test:test@192.168.0.106:5672/my_vhost");
          */

        AMQP amqp(conn_str);

        AMQPQueue * qu2 = amqp.createQueue(queue_name);
        qu2->Declare();

        int i = 0;
        while (  1 ) {
            qu2->Get(AMQP_NOACK);

            AMQPMessage *m = qu2->getMessage();

            if (m->getMessageCount() > -1) {
                uint32_t j = 0;
                i++;
                cout << "Message "<< i << " : "<< m->getMessage(&j) <<  endl;

                if ( include_hdr ) {
                    cout << "message key: "<<  m->getRoutingKey() << endl;
                    cout << "exchange: "<<  m->getExchange() << endl;
                    cout << "Content-type: "<< m->getHeader("Content-type") << endl;
                    cout << "Content-encoding: "<< m->getHeader("Content-encoding") << endl;
                }

            } else
                break;
        }
    } catch (AMQPException &e) {
        std::cout << e.getMessage() << std::endl;
    }

    return 0;

}
