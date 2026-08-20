#include <iostream>
#include <string>
#include <openssl/ssl.h>      // Header file for OpenSSL
#include <amqpcpp.h>          // Core AMQP-CPP header
#include <amqpcpp/libevent.h> // AMQP-CPP libevent adapter
#include <event2/event.h>     // Libevent headers

class myLibEventHandler : public AMQP::LibEventHandler {
public:
    myLibEventHandler(struct event_base *evbase) : LibEventHandler(evbase) {};

    /**
     *  Method that is called right after the TLS connection has been created.
     *  In this method you can check the connection properties (like the certificate)
     *  and return false if you find it not secure enough
     *  @param  connection      the connection that has just completed the tls handshake
     *  @param  ssl             SSL structure from the openssl library
     *  @return bool            true if connection is secure enough to start the AMQP protocol
     */
    virtual bool onSecured(AMQP::TcpConnection *connection, const SSL *ssl) override
    {
        // @todo call functions from the openssl library to check the certificate,
        // like SSL_get_peer_certificate() or SSL_get_verify_result().
        // For now we always allow the connection to proceed
        (void) connection;
        (void) ssl;
        std::cout << "INFO: [TLS] Handshake completed ..." << std::endl;

        // Ensure a certificate was actually presented by the RabbitMQ server
        X509 *cert = SSL_get_peer_certificate(ssl);
        if ( cert == nullptr ) {
            std::cout << "ERROR: [TLS] SSL_get_peer_certificate() failed" << std::endl;
            return false;
        }
        std::cout << "INFO: [TLS] Certificate verified successfully" << std::endl;

        return true;
    }

};

// callback function that is called when the bindQueue operation failed
auto bindQueue_error_cb = [](const char *message) {
    std::cout << "ERROR: bindQueue() - " << message << std::endl;
};

int main(int argc, char * argv[])
{

    if ( argc != 5 ) {
        printf("Usage : %s <url> <exchange> <routing Key> <queue>\n", argv[0]);
        printf("Examle: \n");
        printf("        %s amqps://test:test@10.0.0.191:5671/my_vhost amq.direct rt_key direct_queue\n", argv[0]);
        printf("        %s amqps://test:test@10.0.0.191/my_vhost amq.direct rt_key direct_queue\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::string url_str      = argv[1];
    std::string exchangeName = argv[2];
    std::string routingKey   = argv[3];
    std::string queueName    = argv[4];

    // Add OpenSSL Initialisation
    OPENSSL_init_ssl(0, NULL);              // <-- OpenSSL (libssl and libcrypto) initialisation

    // 1. Initialize the Libevent base (the event loop)
    struct event_base* evbase = event_base_new();

    // 2. Create the AMQP-CPP Libevent Handler
    //    This bridges AMQP-CPP's socket activities with our libevent loop.
    myLibEventHandler handler(evbase);

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
    std::cout << "Program finished Successfully." << std::endl;
    return 0;
}
