#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <amqp.h>
#include <amqp_tcp_socket.h>

#include <assert.h>

#include <string>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

void print_message_header( amqp_message_t msg)
{
    std::cout << "Message Headers : " << std::endl;
    if (msg.properties.content_type.len > 0) {
        std::cout << "Content Type : " << (char*)msg.properties.content_type.bytes << std::endl;
    }
    if (msg.properties.content_encoding.len > 0) {
        std::cout << "Content Encoding : " << (char*)msg.properties.content_encoding.bytes << std::endl;
    }
    if (msg.properties.headers.num_entries > 0) {
        std::cout << "Special Header Entries : " << std::endl;
        for ( int i = 0; i < msg.properties.headers.num_entries; i++ ) {
            std::cout << (char*)msg.properties.headers.entries[i].key.bytes << " : ";
            std::cout << (char*)msg.properties.headers.entries[i].value.value.bytes.bytes << std::endl;
        }
    }
}

void exit_with_error(amqp_connection_state_t conn, int code)
{
    switch ( code ) {
        case 202:
             std::cout << "Failed of creating TCP Socket" << std::endl;
             break;
        case 203:
             std::cout << "failed of opening TCP socket" << std::endl;
             break;
        case 204:
             std::cout << "failed of login" << std::endl;
             break;
        case 205:
             std::cout << "failed of openning channel" << std::endl;
             break;
        case 301:
             std::cout << "failed of declaring Queue" << std::endl;
             break;
        case 302:
             std::cout << "failed of Basic Consuming" << std::endl;
             break;
        case 304:
             std::cout << "failed of reading message" << std::endl;
             break;
        case 400:
             std::cout << "failed of declaring exchange" << std::endl;
             break;
        default:
             break;
    }

    //   amqp_channel_close(conn, my_channel, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
    exit(EXIT_FAILURE);
}

int main(int argc, char const *const *argv) {

    std::vector<std::string> sVec;
    char const *hostname;
    int port;
    amqp_rpc_reply_t rpc_reply;

    if (argc < 3) {
        std::cout <<  "Usage: amqp_get_message <host:port> <queue> [--include_header]\n";
        return 1;
    }

    /*
     *  Step 1 : parse and validate command line syntax
     *       1) argv[1] format <host:port>
     *       2) qu_name = argv[2]
     */

    split(sVec, argv[1], boost::is_any_of(":"));
    hostname = sVec[0].c_str();
    port = std::atoi(sVec[1].c_str());

    amqp_bytes_t qu_name = amqp_cstring_bytes(argv[2]);
    // amqp_bytes_t consumer_tag = amqp_cstring_bytes(argv[4]);
    // amqp_bytes_t consumer_tag = amqp_empty_bytes;

    bool include_hdr = false;
    if ( argc == 4 ) {
        if ( !strcmp(argv[3], "--include_header" ) ) {
            include_hdr = true;
        }
    }

    /*
     *  Step 2 : setup connection
     *     1) amqp_new_connection()
     *     2) amqp_tcp_socket_new()
     *     3) amqp_socket_open()
     *     4) amqp_login()
     *     5) amqp_channel_open()
     */

    amqp_connection_state_t conn =  amqp_new_connection();
      amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if ( socket == nullptr ) {
        exit_with_error(conn, 202);
    }

    int rc = amqp_socket_open(socket, hostname, port);
    if (rc != AMQP_STATUS_OK) {
        exit_with_error(conn, 203);
    }

    rpc_reply = amqp_login(conn, "my_vhost", 0, AMQP_DEFAULT_FRAME_SIZE, 0, AMQP_SASL_METHOD_PLAIN, "test", "test");
    if ( rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) {
        exit_with_error(conn, 204);
    }

    amqp_channel_t my_channel = 1;
    amqp_channel_open(conn, my_channel);
    rpc_reply = amqp_get_rpc_reply(conn);
    if (rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) {
        exit_with_error(conn, 205);
    }

    /*
     *  Step 3 : Declare Queue
     */

    amqp_queue_declare_ok_t *r = amqp_queue_declare( conn, my_channel, qu_name, /*passive*/ 0,
                /*durable*/ 0, /*exclusive*/ 0, /*auto_delete*/ 0, amqp_empty_table);
    if ( r == nullptr) {
        exit_with_error(conn, 301);
    }

    /*
     *  Step 4 : loop get message and print teh message
     */

    for ( uint32_t i = 0; i < r->message_count ; i++) {
        rpc_reply = amqp_basic_get(conn, my_channel, qu_name, /*no_ack*/1);
        amqp_message_t my_msg;
        rpc_reply = amqp_read_message(conn, my_channel, &my_msg, 0);
        if (rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) {
            break;
        }

        std::cout << "Message Get : " << (char*)my_msg.body.bytes << std::endl;
        if ( include_hdr ) {
            print_message_header(my_msg);
        }

        amqp_destroy_message(&my_msg);
    }

    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}
