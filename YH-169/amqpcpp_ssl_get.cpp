#include <AMQPcpp.h>

using namespace std;

int main (int argc, char* argv[]) {

    if (argc != 3)
    {
        printf("Usage: %s <host:port> <queue>\n", argv[0]);
        return 1;
    }

    char const *queue_name = argv[2];
    // Add assgine 3 certificate and key files
    std::string ssl_cacert  = "/etc/rabbitmq/ssl/ca_certificate.pem";
    std::string client_cert = "client_certificate.pem";
    std::string client_key  = "client_key.pem";

    try {

        /*
         *  amqp://[$USERNAME[:$PASSWORD]\@]$HOST[:$PORT]/[$VHOST]
         *  AMQP amqp("test:test@localhost:5672/my_vhost");
         */

        std::string conn_str = "test:test@";
        conn_str += std::string(argv[1]);
        conn_str += std::string("/my_vhost");
        // Add Modify AMQP constructor to enable TLS/SSL
        AMQP amqp(conn_str, true, ssl_cacert, client_cert, client_key, false, false);

        short qu2_param = AMQP_DURABLE;
        AMQPQueue * qu2 = amqp.createQueue(queue_name);
        qu2->Declare(queue_name, qu2_param);

        while (  1 ) {
            qu2->Get(AMQP_NOACK);

            AMQPMessage *m = qu2->getMessage();

            printf("count: %d\n", m->getMessageCount());
            // cout << "count: "<<  m->getMessageCount() << endl;
            if (m->getMessageCount() > -1) {
                uint32_t j = 0;
                cout << "message : " << m->getMessage(&j) << "\nmessage key: "<<  m->getRoutingKey() << e
                cout << "exchange: " <<  m->getExchange() << endl;
                cout << "Content-type: " << m->getHeader("Content-type") << endl;
                cout << "Content-encoding: " << m->getHeader("Content-encoding") << endl;
            } else
                break;
        }
    } catch (AMQPException &e) {
         std::cout << e.getMessage() << std::endl;
    }

    return 0;

}
