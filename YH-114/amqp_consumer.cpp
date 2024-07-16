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
#include <time.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#define MESSAGE_BUFFER_SIZE     1024
#define ENVELOPE_BUFFER_SIZE    256

/*
 * #define AMQP_BASIC_CLASS (0x003C) // basic class id @internal 60
 * #define AMQP_BASIC_CONTENT_TYPE_FLAG (1 << 15)
 * #define AMQP_BASIC_CONTENT_ENCODING_FLAG (1 << 14)
 * #define AMQP_BASIC_HEADERS_FLAG (1 << 13)
 * #define AMQP_BASIC_DELIVERY_MODE_FLAG (1 << 12)
 * #define AMQP_BASIC_PRIORITY_FLAG (1 << 11)
 * #define AMQP_BASIC_CORRELATION_ID_FLAG (1 << 10)
 * #define AMQP_BASIC_REPLY_TO_FLAG (1 << 9)
 * #define AMQP_BASIC_EXPIRATION_FLAG (1 << 8)
 * #define AMQP_BASIC_MESSAGE_ID_FLAG (1 << 7)
 * #define AMQP_BASIC_TIMESTAMP_FLAG (1 << 6)
 * #define AMQP_BASIC_TYPE_FLAG (1 << 5)
 * #define AMQP_BASIC_USER_ID_FLAG (1 << 4)
 * #define AMQP_BASIC_APP_ID_FLAG (1 << 3)
 * #define AMQP_BASIC_CLUSTER_ID_FLAG (1 << 2)
 */

void print_message(amqp_message_t msg)
{
    /*
     *   amqp_message_t object include following information
     *      1) Basic Header information
     *      2) Special Header information
     *      3) Message Body Information
     */

    char msg_buffer[MESSAGE_BUFFER_SIZE];

    std::cout << "***** Basic Message Hesder ***** " << std::endl;

    if ( msg.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG &&
         msg.properties.content_type.len > 0 ) {
        memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
        memcpy(msg_buffer, msg.properties.content_type.bytes, msg.properties.content_type.len);
        std::cout << "Content Type : " << msg_buffer << std::endl;
    }

    if ( msg.properties._flags & AMQP_BASIC_CONTENT_ENCODING_FLAG  &&
         msg.properties.content_encoding.len > 0 ) {
        memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
        memcpy(msg_buffer, msg.properties.content_encoding.bytes, msg.properties.content_encoding.len);
        std::cout << "Content Encoding : " <<  msg_buffer << std::endl;
    }

    if ( msg.properties._flags & AMQP_BASIC_APP_ID_FLAG &&
         msg.properties.app_id.len > 0 ) {
        memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
        memcpy(msg_buffer, msg.properties.app_id.bytes, msg.properties.app_id.len);
        std::cout << "Application ID : " <<  msg_buffer << std::endl;
    }

    if ( msg.properties._flags & AMQP_BASIC_CLUSTER_ID_FLAG &&
         msg.properties.cluster_id.len > 0 ) {
        memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
        memcpy(msg_buffer, msg.properties.cluster_id.bytes, msg.properties.cluster_id.len);
        std::cout << "Cluster ID : " <<  msg_buffer << std::endl;
    }

    if ( msg.properties._flags & AMQP_BASIC_CORRELATION_ID_FLAG &&
         msg.properties.correlation_id.len > 0 ) {
        memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
        memcpy(msg_buffer, msg.properties.correlation_id.bytes, msg.properties.correlation_id.len);
        std::cout << "Correlation ID : " <<  msg_buffer << std::endl;
    }

    if ( msg.properties._flags & AMQP_BASIC_EXPIRATION_FLAG &&
         msg.properties.expiration.len > 0 ) {
        memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
        memcpy(msg_buffer, msg.properties.expiration.bytes, msg.properties.expiration.len);
        std::cout << "Expiration : " <<  msg_buffer << std::endl;
    }

    if ( msg.properties._flags & AMQP_BASIC_MESSAGE_ID_FLAG &&
         msg.properties.message_id.len > 0 ) {
        memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
        memcpy(msg_buffer, msg.properties.message_id.bytes, msg.properties.message_id.len);
        std::cout << "Message ID : " <<  msg_buffer << std::endl;
    }

    if ( msg.properties._flags & AMQP_BASIC_TYPE_FLAG &&
         msg.properties.type.len > 0 ) {
        memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
        memcpy(msg_buffer, msg.properties.type.bytes, msg.properties.type.len);
        std::cout << "Message Type : " <<  msg_buffer << std::endl;
    }

    if ( msg.properties._flags & AMQP_BASIC_USER_ID_FLAG &&
         msg.properties.user_id.len > 0 ) {
        memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
        memcpy(msg_buffer, msg.properties.user_id.bytes, msg.properties.user_id.len);
        std::cout << "User ID : " <<  msg_buffer << std::endl;
    }

    if ( msg.properties._flags & AMQP_BASIC_REPLY_TO_FLAG &&
         msg.properties.reply_to.len > 0 ) {
        memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
        memcpy(msg_buffer, msg.properties.reply_to.bytes, msg.properties.reply_to.len);
        std::cout << "Reply to : " <<  msg_buffer << std::endl;
    }

    if ( msg.properties._flags & AMQP_BASIC_DELIVERY_MODE_FLAG ) {
        // std::cout << "Delivery Mode : " << (uint8_t)msg.properties.delivery_mode << std::endl;
    }

    if ( msg.properties._flags & AMQP_BASIC_PRIORITY_FLAG ) {
        // std::cout << "Priority : " << (uint8_t)msg.properties.priority << std::endl;
    }

    if ( msg.properties._flags & AMQP_BASIC_TIMESTAMP_FLAG ) {
        std::cout << "Time Stamp : " << std::ctime((time_t*)&msg.properties.timestamp) << std::endl;
    }

    if ( msg.properties.headers.num_entries > 0 ) {
        std::cout << "***** Special Header Entries ***** " << std::endl;
        for ( int i = 0; i < msg.properties.headers.num_entries; i++) {
            memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
            memcpy(msg_buffer, msg.properties.headers.entries[i].key.bytes, msg.properties.headers.entrie
            std::cout << msg_buffer << " : ";
            memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
            memcpy(msg_buffer, msg.properties.headers.entries[i].value.value.bytes.bytes,
                   msg.properties.headers.entries[i].value.value.bytes.len);
            std::cout << msg_buffer << std::endl;
        }
    }
    memset(msg_buffer, '\0', MESSAGE_BUFFER_SIZE);
    memcpy(msg_buffer, msg.body.bytes, msg.body.len);
    std::cout << "Message Body : " << msg_buffer  << std::endl;
}

void print_envelope(amqp_envelope_t envelope)
{
    /*
     * Envelope object include following information
     *   1) Channel ID
     *   2) Consumer Tag
     *   3) Delivery Tag
     *   4) Exchange Name
     *   5) Routing Key
     *   6) Redelivered (true/false)
     *   7) Message Info
     */

    char buffer[ENVELOPE_BUFFER_SIZE];

    std::cout << "Channel ID : " << envelope.channel << std::endl;

    memset(buffer, '\0', ENVELOPE_BUFFER_SIZE);
    memcpy(buffer, envelope.consumer_tag.bytes, envelope.consumer_tag.len);
    std::cout << "Consumer Tag : " << buffer;
    std::cout << " Delivery Tag : " << envelope.delivery_tag << std::endl;

    memset(buffer, '\0', ENVELOPE_BUFFER_SIZE);
    memcpy(buffer, envelope.exchange.bytes, envelope.exchange.len);
    memcpy(buffer, envelope.exchange.bytes, envelope.exchange.len);
    std::cout << "Exchange Name : " << buffer << std::endl;

    memset(buffer, '\0', ENVELOPE_BUFFER_SIZE);
    memcpy(buffer, envelope.routing_key.bytes, envelope.routing_key.len);
    std::cout << "Routing Key : " << buffer << std::endl;

    print_message(envelope.message);
}

static void run_consumer(amqp_connection_state_t conn) {

    // amqp_frame_t frame;
    for (;;) {

        amqp_rpc_reply_t rpc_reply;
        amqp_envelope_t envelope;
        amqp_maybe_release_buffers(conn);

        /*
         * Wait for consume a message (wait for basic delivery)
         * amqp_consume_message() Allocate memeory for envelope object
         */

        rpc_reply = amqp_consume_message(conn, &envelope, NULL, 0);
        if ( rpc_reply.reply_type == AMQP_RESPONSE_NORMAL ) {
            print_envelope(envelope);
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

    // amqp_channel_close(conn, channel_id, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
    exit(EXIT_FAILURE);
}

void print_usage()
{
    std::cout <<  "Usage: amqp_consumer <host:port> <queue> [consumer tag]\n";
    exit(EXIT_FAILURE);
}

int main(int argc, char const *const *argv) {

    std::vector<std::string> sVec;
    char const *hostname;
    int port;
    amqp_rpc_reply_t rpc_reply;

    if (argc < 3) { print_usage(); }

    /*
     *  Step 1 : parse and validate command line syntax
     *       1) argv[1] format <host:port>
     *       2) qu_name = argv[2]
     *       3) consum_tag = argv[3]
     */

    sVec.clear();
    boost::split(sVec, argv[1], boost::is_any_of(":"));
    if ( sVec.size() != 2 ) { print_usage(); }
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

    rpc_reply = amqp_login(conn, "my_vhost", 0, AMQP_DEFAULT_FRAME_SIZE, 0, AMQP_SASL_METHOD_PLAIN, "test
    if ( rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) {
        exit_with_error(conn, 204);
    }

    amqp_channel_t channel_id = 1;
    amqp_channel_open(conn, channel_id);
    rpc_reply = amqp_get_rpc_reply(conn);
    if (rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) {
        exit_with_error(conn, 205);
    }

    /*
     *  Step 3 : Declare Queue and register consumer tag
     *         1) amqp_queue_declare()   <- Declare queue
     *         2) amqp_basic_consume()   <- Start Consumer
     */

    amqp_queue_declare_ok_t *r = amqp_queue_declare( conn, channel_id, qu_name, /*passive*/ 0,
                /*durable*/ 0, /*exclusive*/ 0, /*auto_delete*/ 0, amqp_empty_table);
    if ( r == nullptr) {
        exit_with_error(conn, 301);
    }

    amqp_basic_consume(conn, channel_id, qu_name, consumer_tag, /*no_local*/ 0,
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
