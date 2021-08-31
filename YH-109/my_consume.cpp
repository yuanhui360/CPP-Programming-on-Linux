#include <iostream>
#include <AMQPcpp.h>

using namespace std;

int i=0;

int onCancel(AMQPMessage * message ) {
    cout << "cancel tag="<< message->getDeliveryTag() << endl;
    return 0;
}

int  onMessage( AMQPMessage * message  ) {
    uint32_t j = 0;
    char * data = message->getMessage(&j);
    if (data)
          cout << data << endl;

    i++;
    cout << "#" << i << " tag="<< message->getDeliveryTag()
         << " content-type:"<< message->getHeader("Content-type") ;
    cout << " encoding:"<< message->getHeader("Content-encoding")
         << " mode="<<message->getHeader("Delivery-mode")<<endl;

    if ( std::string(data) == "stop" ) {
        AMQPQueue * q = message->getQueue();
        q->Cancel( message->getConsumerTag() );
    }
    return 0;
};

int main (int argc, char* argv[]) {

    if (argc != 4)
    {
        std::cout << "Usage: my_get <host:port> <exchange> <queue>" << std::endl;
        std::cout << "Example :" << std::endl;
        std::cout << "my_consume \"192.168.0.106:5672\"  my_exchange my_queue" << std::endl;
        return 1;
    }

    std::string conn_str = "test:test@";
    conn_str += std::string(argv[1]);
    conn_str += std::string("/my_vhost");

    char const *exchange_name = argv[2];
    char const *queue_name = argv[3];

    try {

        AMQP amqp(conn_str);

        AMQPQueue * qu2 = amqp.createQueue(queue_name);

        qu2->Declare();
        qu2->Bind( exchange_name, "");

        qu2->setConsumerTag("tag_123");
        qu2->addEvent(AMQP_MESSAGE, onMessage );
        qu2->addEvent(AMQP_CANCEL, onCancel );

        qu2->Consume(AMQP_NOACK);


    } catch (AMQPException &e) {
        std::cout << e.getMessage() << std::endl;
    }

    return 0;

}
