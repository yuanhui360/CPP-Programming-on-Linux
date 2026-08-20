#include <iostream>
#include <string>
#include <amqpcpp.h>          // Core AMQP-CPP header
#include <amqpcpp/libevent.h> // AMQP-CPP libevent adapter
#include <event2/event.h>     // Libevent headers

// callback function that is called when the bindQueue operation failed
auto declareQueue_error_cb = [](const char *message) {
    std::cout << "ERROR: declareQueue() - " << message << std::endl;
};

int main(int argc, char * argv[])
{

    if ( argc != 3 ) {
        printf("Usage : %s <url> <queue>\n", argv[0]);
        printf("Examle: \n");
        printf("        %s amqp://test:test@10.0.0.191:5672/my_vhost direct_queue\n", argv[0]);
        printf("        %s amqp://test:test@10.0.0.191/my_vhost direct_queue\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::string url_str      = argv[1];
    std::string queueName    = argv[2];

    // 1. Initialize the Libevent base (the event loop)
    struct event_base* evbase = event_base_new();

    // 2. Create the AMQP-CPP Libevent Handler
    //    This bridges AMQP-CPP's socket activities with our libevent loop.
    AMQP::LibEventHandler handler(evbase);

    // 3. Define RabbitMQ Connection Details
    //    Format: amqp://username:password@hostname:port/vhost
    AMQP::Address address(url_str);
    std::cout << "INFO : Hostname - " << address.hostname() << std::endl;
    std::cout << "     : Login    - " << address.login() << std::endl;
    std::cout << "     : Portn    - " << address.port() << std::endl;
    std::cout << "     : Vhost    - " << address.vhost() << std::endl;

    // 4. Create connection object -> Connecting to RabbitMQ
    std::cout << "Connect to RabbitMQ..." << std::endl;
    AMQP::TcpConnection connection(&handler, address);

    // 5. Open a channel over the connection
    AMQP::TcpChannel channel(&connection);

    // 6. use the channel object to call the AMQP method :
    //    channel.declareExchange("my-exchange", AMQP::direct);
    //    channel.declareQueue("my-queue");
    //    channel.bindQueue("my-exchange", "my-queue", "my-routing-key");
    channel.declareQueue(queueName, AMQP::durable)
    .onSuccess([&channel](const std::string &name, uint32_t messageCount, uint32_t consumerCount) {
            std::cout << "INFO: channel.declareQueue().onSuccess() - Started " << std::endl;
            std::cout << "    : Queue Name      - " << name << std::endl;
            std::cout << "    : Message  Count  - " << messageCount << std::endl;
            std::cout << "    : Consumer Count  - " << consumerCount << std::endl;

            // Add consume message process
            if ( messageCount > 0 ) {
                channel.consume(name, AMQP::noack)
                .onSuccess([](const std::string &consumertag) {
                    std::cout << "INFO: channel.consume().onSuccess() - " << consumertag << std::endl;
                })
                .onReceived([](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) {
                    (void) redelivered;
                    char message_buf[1024];
                    memset(message_buf, '\0', 1024);
                    memcpy(message_buf, message.body(), message.bodySize());
                    std::cout << "INFO: channel.consume().onReceived() (" << deliveryTag << "-" <<
                                 message.bodySize() << ") " << message_buf;
                    std::cout << std::endl;
                })
                .onDelivered([](uint64_t deliveryTag, bool redelivered) {
                    std::cout << "INFO: channel.consume().onDelivered() (" << deliveryTag << ")";
                    if ( redelivered ) {
                        std::cout << " Redelivered True";
                    } else {
                        std::cout << " Redelivered False";
                    }
                    std::cout << std::endl;
                })
                .onFinalize([&channel]() {
                    std::cout << "INFO: channel.consume().onFinalize() - Started" << std::endl;
                    channel.close().onSuccess([](){
                        std::cout << "INFO: channel.close().onSuccess() - Success" << std::endl;
                    });
                })
                .onError([](const char *message) {
                    std::cout << "ERROR: channel.consume().onError() - " << message << std::endl;
                });
            } else {
                std::cout << "INFO: channel.declareQueue().onSuccess() - Message Count : " << messageCount << std::endl;
            }

    })
    .onFinalize([&connection](){
        std::cout << "INFO: channel.declareQueue().onFinalize() - started" << std::endl;
        if ( connection.usable() ) {
            if ( connection.close() ) {
                std::cout << "INFO: channel.declareQueue().onFinalize() - Connection closed" << std::endl;
            }
        }
    })
    .onError(declareQueue_error_cb);

    // 7. Run the event loop
    //    This call blocks and keeps running until `event_base_loopbreak` is called.
    event_base_dispatch(evbase);

    // 8. Cleanup
    event_base_free(evbase);
    std::cout << "Program finished Successfully." << std::endl;
    return 0;
}
