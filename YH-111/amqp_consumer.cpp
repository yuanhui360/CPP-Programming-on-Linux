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

void print_message(amqp_message_t msg)
{
    char msg_buffer[1024];
    std::cout << "Message Hesder : " << std::endl;
    if ( msg.properties.content_type.len > 0 ) {
        memset(msg_buffer, '\0', sizeof(msg_buffer));
        memcpy(msg_buffer, msg.properties.content_type.bytes, msg.properties.content_type.len);
        std::cout << "Content Type : " << msg_buffer << std::endl;
    }
    if ( msg.properties.content_encoding.len > 0 ) {
        memset(msg_buffer, '\0', sizeof(msg_buffer));
        memcpy(msg_buffer, msg.properties.content_encoding.bytes, msg.properties.content_encoding.len);
        std::cout << "Content Encoding : " <<  msg_buffer << std::endl;
    }
    if ( msg.properties.headers.num_entries > 0 ) {
        std::cout << "Special Header Entries : " << std::endl;
        for ( int i = 0; i<msg.properties.headers.num_entries; i++) {
            memset(msg_buffer, '\0', sizeof(msg_buffer));
            memcpy(msg_buffer, msg.properties.headers.entries[i].key.bytes, msg.properties.headers.entries[i].key.len);
            std::cout << msg_buffer << " : ";
            memset(msg_buffer, '\0', sizeof(msg_buffer));
            memcpy(msg_buffer, msg.properties.headers.entries[i].value.value.bytes.bytes,
                   msg.properties.headers.entries[i].value.value.bytes.len);
            std::cout << msg_buffer << std::endl;
        }
    }
    memset(msg_buffer, '\0', sizeof(msg_buffer));
    memcpy(msg_buffer, msg.body.bytes, msg.body.len);
    std::cout << "Message Body : " << msg_buffer  << std::endl;
}

void print_envelope(amqp_envelope_t envelope)
{
    char buffer[256];
    std::cout << "Channel ID : " << envelope.channel << std::endl;
    memset(buffer, '\0', sizeof(buffer));
    memcpy(buffer, envelope.consumer_tag.bytes, envelope.consumer_tag.len);
    std::cout << "Consumer Tag : " << buffer;
    std::cout << " Delivery Tag : " << envelope.delivery_tag << std::endl;
    memset(buffer, '\0', sizeof(buffer));
    memcpy(buffer, envelope.exchange.bytes, envelope.exchange.len);
    std::cout << "Exchange Name : " << buffer << std::endl;
    memset(buffer, '\0', sizeof(buffer));
    memcpy(buffer, envelope.routing_key.bytes, envelope.routing_key.len);
    std::cout << "Routing Key : " << buffer << std::endl;

    print_message(envelope.message);
}

static void run_consumer(amqp_connection_state_t conn) {

    amqp_frame_t frame;
    for (;;) {

        amqp_rpc_reply_t rpc_reply;
        amqp_envelope_t envelope;
        amqp_maybe_release_buffers(conn);

        /*
         * Wait for consume a message
         * amqp_consume_message() Allocate memeory for envelope object
         */
        rpc_reply = amqp_consume_message(conn, &envelope, NULL, 0);
        if ( rpc_reply.reply_type == AMQP_RESPONSE_NORMAL ) {
            print_envelope(envelope);
        }
        else {
            if ( rpc_reply.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION &&
                 rpc_reply.library_error == AMQP_STATUS_UNEXPECTED_STATE) {
                /*
                 * Read Single Frame from broker
                 */
                if ( amqp_simple_wait_frame(conn, &frame) != AMQP_STATUS_OK) {
                    break;
                }
                if ( frame.frame_type == AMQP_FRAME_METHOD ) {
                    if ( frame.payload.method.id == AMQP_BASIC_RETURN_METHOD ) {
                        amqp_message_t msg;
                        rpc_reply = amqp_read_message(conn, frame.channel, &msg, 0);
                        if ( rpc_reply.reply_type != AMQP_RESPONSE_NORMAL ) {
                            break;
                        }
                        print_message(msg);
                        amqp_destroy_message(&msg);
                    }
                }
            }
        }

        amqp_destroy_envelope(&envelope);
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
        case 400:
             std::cout << "failed of declaring exchange" << std::endl;
             break;
        default:
             break;
    }

    // amqp_channel_close(conn, my_channel, AMQP_REPLY_SUCCESS);
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
        std::cout <<  "Usage: amqp_consumer <host:port> <queue> [consumer tag]\n";
        return 1;
    }

    /*
     *  Step 1 : parse and validate command line syntax
     *       1) argv[1] format <host:port>
     *       2) qu_name = argv[2]
     *       3) consum_tag = argv[3]
     */

    split(sVec, argv[1], boost::is_any_of(":"));
    hostname = sVec[0].c_str();
    port = std::atoi(sVec[1].c_str());

    amqp_bytes_t qu_name = amqp_cstring_bytes(argv[2]);
    amqp_bytes_t consumer_tag = amqp_empty_bytes;
    if (argc == 4) {
        consumer_tag = amqp_cstring_bytes(argv[3]);
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
        amqp_destroy_connection(conn);
        std::cout << "failed of login" << std::endl;
    }

    amqp_channel_t my_channel = 1;
    amqp_channel_open(conn, my_channel);
    rpc_reply = amqp_get_rpc_reply(conn);
    if (rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) {
        exit_with_error(conn, 205);
    }

    /*
     *  Step 3 : Declare Queue and register consumer tag
     *         1) amqp_queue_declare()   <- Declare queue
     *         2) amqp_basic_consume()   <- Start Consumer
     */

    amqp_queue_declare_ok_t *r = amqp_queue_declare( conn, my_channel, qu_name, /*passive*/ 0,
                /*durable*/ 0, /*exclusive*/ 0, /*auto_delete*/ 0, amqp_empty_table);
    if ( r == nullptr) {
        exit_with_error(conn, 301);
    }

    amqp_basic_consume(conn, my_channel, qu_name, consumer_tag, /*no_local*/ 0,
                       /*no_ack*/ 1, /*exclusive*/ 0, amqp_empty_table);
    rpc_reply = amqp_get_rpc_reply(conn);
    if (rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) {
        exit_with_error(conn, 302);
    }

    run_consumer(conn);

    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}
      
