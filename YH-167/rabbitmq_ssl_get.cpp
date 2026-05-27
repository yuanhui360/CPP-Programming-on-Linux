#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/ssl_socket.h>

#include <assert.h>

#include <string>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#define MESSAGE_BUFFER_SIZE     1024

/*
 *  Get message from RabbitMQ brocker
 */

int main(int argc, char* argv[]) {

    std::vector<std::string> sVec;

    if (argc < 4) {
        printf("Usage: %s <host:port> <queue> <ca Cert>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
     *  Step 1 : Parse and validate connamd line argument
     */

    sVec.clear();
    boost::split(sVec, argv[1], boost::is_any_of(":"));
    if ( sVec.size() != 2 ) {
        printf("Usage: %s <host:port> <queue>\n", argv[0]);
        std::cout << "Example :" << std::endl;
        std::cout << argv[0] << " 192.168.0.103:5672 direct_queue" << std::endl;
        exit(EXIT_FAILURE);
    }

    const char *hostname = sVec[0].c_str();
    int port = std::atoi(sVec[1].c_str());
    amqp_bytes_t qu_name = amqp_cstring_bytes(argv[2]);

    /*
     *  Step 2 : setup connection
     *     1) amqp_new_connection()
     *     2) amqp_tcp_socket_new()
     *     3) amqp_socket_open()
     *     4) amqp_login()
     *     5) amqp_channel_open()
     */

    amqp_connection_state_t conn =  amqp_new_connection();
    amqp_socket_t *socket = amqp_ssl_socket_new(conn);
    if ( socket == nullptr ) {
        printf("ERROR: amqp_new_connection() or amqp_tcp_socket_new() Failed\n");
        exit(EXIT_FAILURE);
    }

    if ( amqp_ssl_socket_set_cacert(socket, argv[3]) != AMQP_STATUS_OK) {
        printf("ERROR: amqp_ssl_socket_set_cacert() Failed\n");
        exit(EXIT_FAILURE);
    }
    amqp_ssl_socket_set_verify_peer(socket, false);
    amqp_ssl_socket_set_verify_hostname(socket, false);
    printf("INFO: step 2-A : amqp_ssl_socket_set_cacert() Success\n");

    int rc = amqp_socket_open(socket, hostname, port);
    if (rc != AMQP_STATUS_OK) {
    }

    amqp_rpc_reply_t res = amqp_login(conn, "my_vhost", 0, AMQP_DEFAULT_FRAME_SIZE, 0,
                                      AMQP_SASL_METHOD_PLAIN, "test", "test");
    if ( res.reply_type != AMQP_RESPONSE_NORMAL) {
    }

    amqp_channel_t channel_id = 1;
    amqp_channel_open(conn, channel_id);
    res = amqp_get_rpc_reply(conn);
    if (res.reply_type != AMQP_RESPONSE_NORMAL) {
        printf("ERROR: amqp_channel_open() Failed\n");
        exit(EXIT_FAILURE);
    }
    printf("INFO: amqp_channel_open() Success\n");

    /*
     *  Step 3 : Declare Queue
     *       amqp_queue_declare()
     */

    amqp_queue_declare_ok_t *r = amqp_queue_declare(
        conn,                        // Connection state
        channel_id,                  // Channel ID
        qu_name,                     // Queue name (as amqp_bytes_t)
        0,                           // Passive (0 = false; error if it doesn't exist)
        1,                           // Durable (1 = true; survives broker restart)
        0,                           // Exclusive (0 = false; queue can be shared)
        0,                           // Auto-delete (0 = false; won't delete when empty)
        amqp_empty_table             // Arguments (e.g., custom headers/dead lettering)
    );

    res = amqp_get_rpc_reply(conn);
    if (res.reply_type != AMQP_RESPONSE_NORMAL) {
        printf("ERROR: amqp_queue_declare() Failed\n");
        exit(EXIT_FAILURE);
    }
    printf("INFO: amqp_queue_declare() Success\n");

    /*
     *  Step 4 : loop get message and print teh message
     *      loop:
     *         amqp_basic_get()    <- Synchonously polls the broker for a message in a queue
     *         amqp_read_message() <- Reads the next message on a channel
     */

    char msg_buffer[MESSAGE_BUFFER_SIZE];
    memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);

    for ( uint32_t i = 0; i < r->message_count ; i++) {
        res = amqp_basic_get(conn, channel_id, qu_name, /*no_ack*/1);
        amqp_message_t my_msg;
        res = amqp_read_message(conn, channel_id, &my_msg, 0);
        if (res.reply_type != AMQP_RESPONSE_NORMAL) {
            break;
        }

        memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
        memcpy(msg_buffer, my_msg.body.bytes, my_msg.body.len);
        std::cout << "Message Body : " << msg_buffer << std::endl;

        amqp_destroy_message(&my_msg);
    }

    /*
     * Step 5. Cleanup and Close Connection
     */

    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return EXIT_SUCCESS;
}
