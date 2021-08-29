#include <AMQPcpp.h>

using namespace std;

int main (int argc, char* argv[]) {

    if (argc != 2)
    {
        std::cout << "Usage: my_get <queue>" << std::endl;
        return 1;
    }

    char const *queue_name = argv[1];

    try {

        /*
         *  amqp://[$USERNAME[:$PASSWORD]\@]$HOST[:$PORT]/[$VHOST]
         */

        AMQP amqp("test:test@localhost:5672/my_vhost");

        AMQPQueue * qu2 = amqp.createQueue(queue_name);
        qu2->Declare();

        while (  1 ) {
            qu2->Get(AMQP_NOACK);

            AMQPMessage *m = qu2->getMessage();

            cout << "count: "<<  m->getMessageCount() << endl;
            if (m->getMessageCount() > -1) {
                uint32_t j = 0;
                cout << "message\n"<< m->getMessage(&j) << "\nmessage key: "<<  m->getRoutingKey() << endl;
                cout << "exchange: "<<  m->getExchange() << endl;
                cout << "Content-type: "<< m->getHeader("Content-type") << endl;
                cout << "Content-encoding: "<< m->getHeader("Content-encoding") << endl;
            } else
                break;
        }
    } catch (AMQPException &e) {
        std::cout << e.getMessage() << std::endl;
    }

    return 0;

}
