#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/ssl_socket.h>

#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#define MESSAGE_BUFFER_SIZE     1024

/*
 *  Check Queue Status
 */

int main(int argc, char* argv[]) {

    std::vector<std::string> sVec;
    int status;
    amqp_socket_t *socket = NULL;
    amqp_connection_state_t conn;

    if (argc < 4) {
        printf("Usage: %s <host:port> <queue> <CA cert>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sVec.clear();
    boost::split(sVec, argv[1], boost::is_any_of(":"));
    if ( sVec.size() != 2 ) {
        printf("Usage: %s <host:port> <queue>\n", argv[0]);
        printf("Example :\n");
        printf(" %s 192.168.0.103:5672 direct_queue\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *hostname = sVec[0].c_str();
    int port = std::atoi(sVec[1].c_str());
    amqp_bytes_t qu_name = amqp_cstring_bytes(argv[2]);

    // 1. Initialize connection state
    conn = amqp_new_connection();

    // 2. Open a TCP socket to localhost (default RabbitMQ port 5672)
    socket = amqp_ssl_socket_new(conn);
    if (!socket) {
        fprintf(stderr, "Error creating TCP socket\n");
        return 1;
    }

    if ( amqp_ssl_socket_set_cacert(socket, argv[3]) != AMQP_STATUS_OK) {
        printf("ERROR: amqp_ssl_socket_set_cacert() Failed\n");
        exit(EXIT_FAILURE);
    }
    amqp_ssl_socket_set_verify_peer(socket, false);
    amqp_ssl_socket_set_verify_hostname(socket, false);
    printf("INFO: step 2-A : amqp_ssl_socket_set_cacert() Success\n");

    status = amqp_socket_open(socket, hostname, port);
    if (status != AMQP_STATUS_OK) {
        fprintf(stderr, "ERROR : Error opening TCP socket\n");
        return 1;
    }

    // 3. Login to the RabbitMQ broker (using default guest/guest credentials)
    amqp_rpc_reply_t login_reply = amqp_login(
        conn, "my_vhost", 0, AMQP_DEFAULT_FRAME_SIZE, 0,
        AMQP_SASL_METHOD_PLAIN, "test", "test"
    );
    if (login_reply.reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "ERROR : Login failed\n");
        return 1;
    }

    // 4. Open Channel 1
    amqp_channel_open(conn, 1);
    amqp_rpc_reply_t channel_reply = amqp_get_rpc_reply(conn);
    if (channel_reply.reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "ERROR : Failed to open channel\n");
        return 1;
    }

    // 5. USE amqp_queue_declare() TO DECLARE A QUEUE
    printf("Declaring queue...\n");

    amqp_queue_declare_ok_t *r = amqp_queue_declare(
        conn,                        // Connection state
        1,                           // Channel ID
        qu_name,                     // Queue name (as amqp_bytes_t)
        0,                           // Passive (0 = false; error if it doesn't exist)
        1,                           // Durable (1 = true; survives broker restart)
        0,                           // Exclusive (0 = false; queue can be shared)
        0,                           // Auto-delete (0 = false; won't delete when empty)
        amqp_empty_table             // Arguments (e.g., custom headers/dead lettering)
    );

    // 6. Verify if the declaration was successful
    amqp_rpc_reply_t declare_reply = amqp_get_rpc_reply(conn);
    if (declare_reply.reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "ERROR : Queue declaration failed!\n");
    } else {
        // Double check by reading the server reply structure
        printf("Success! Declared queue: %.*s\n", (int)r->queue.len, (char *)r->queue.bytes);
        printf("Current message count: %u\n", r->message_count);
        printf("Current consumer count: %u\n", r->consumer_count);
    }

    // 7. Cleanup and Close Connection
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}
