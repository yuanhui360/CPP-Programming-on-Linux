#include <iostream>
#include <string>
#include <amqpcpp.h>          // Core AMQP-CPP header
#include <amqpcpp/libevent.h> // AMQP-CPP libevent adapter
#include <event2/event.h>     // Libevent headers

// callback function that is called when the bindQueue operation failed
auto bindQueue_error_cb = [](const char *message) {
    std::cout << "ERROR: bindQueue() - " << message << std::endl;
};

int main(int argc, char * argv[])
{

    if ( argc != 5 ) {
        printf("Usage : %s <url> <exchange> <routing Key> <queue>\n", argv[0]);
        printf("Examle: \n");
        printf("        %s amqp://test:test@10.0.0.191:5672/my_vhost amq.direct rt_key direct_queue\n", argv[0]);
        printf("        %s amqp://test:test@10.0.0.191/my_vhost amq.direct rt_key direct_queue\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::string url_str      = argv[1];
    std::string exchangeName = argv[2];
    std::string routingKey   = argv[3];
    std::string queueName    = argv[4];

    // 1. Initialize the Libevent base (the event loop)
    struct event_base* evbase = event_base_new();

    // 2. Create the AMQP-CPP Libevent Handler
    //    This bridges AMQP-CPP's socket activities with our libevent loop.
    AMQP::LibEventHandler handler(evbase);

    // 3. Define RabbitMQ Connection Details
    //    Format: amqp://username:password@hostname:port/vhost
    AMQP::Address address(url_str);
    std::cout << "INFO : Hostname - " << address.hostname() << std::endl;
    std::cout << "INFO : Login    - " << address.login() << std::endl;
    std::cout << "INFO : Portn    - " << address.port() << std::endl;
    std::cout << "INFO : Vhost    - " << address.vhost() << std::endl;

    // 4. Create connection object -> Connecting to RabbitMQ
    std::cout << "Connect to RabbitMQ..." << std::endl;
    AMQP::TcpConnection connection(&handler, address);

    // 5. Open a channel over the connection
    AMQP::TcpChannel channel(&connection);

    // 6. use the channel object to call the AMQP method :
    //    channel.declareExchange("my-exchange", AMQP::direct);
    //    channel.declareQueue("my-queue");
    //    channel.bindQueue("my-exchange", "my-queue", "my-routing-key");
    channel.bindQueue(exchangeName, queueName, routingKey)
        .onSuccess([&connection, &channel, exchangeName, queueName, routingKey]() {
            std::cout << "INFO: channel.bindQueue() Success " << std::endl;

            for (;;) {
                std::string msg_string;
                std::cout << "Please input Message : ";
                std::getline(std::cin, msg_string);
                if ( msg_string == "end" ) {
                    break;
                }
                channel.publish(exchangeName, routingKey, msg_string);
            }
            connection.close();
        })
    .onError(bindQueue_error_cb);

    // 7. Run the event loop
    //    This call blocks and keeps running until `event_base_loopbreak` is called.
    event_base_dispatch(evbase);

    // 8. Cleanup
    event_base_free(evbase);
    std::cout << "Program finished cleanly." << std::endl;
    return 0;
}
