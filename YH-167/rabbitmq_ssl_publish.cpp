#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/ssl_socket.h>

#include <string>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

/*
 * Example of Publishing message to RabbitMQ using direct exhange
 */

int main(int argc, char *argv[])
{
    std::vector<std::string> sVec;

    if (argc < 6) {
        printf("Usage: %s <host:port> <exchange> <rounting key> [queue] <CA Cert File>\n", argv[0]);
        std::cout << "Example :" << std::endl;
        std::cout << "amqp_publish 192.168.0.103:5672 amq.direct rt_key direct_queue ca_certificate.pem" << std::endl;
        exit(EXIT_FAILURE);
    }

    /*
     *  Step 1 : Parse and validate connamd line argument
     */

    sVec.clear();
    boost::split(sVec, argv[1], boost::is_any_of(":"));
    if ( sVec.size() != 2 ) {
        printf("Usage: %s <host:port> <exchange> <rounting key> [queue]\n", argv[0]);
        std::cout << "Example :" << std::endl;
        printf( "  %s 10.0.0.191:5672 yh_exchange rt_key direct_queue\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *hostname = sVec[0].c_str();
    int port = std::atoi(sVec[1].c_str());
    amqp_bytes_t ex_name = amqp_cstring_bytes(argv[2]);
    amqp_bytes_t rt_key  = amqp_cstring_bytes(argv[3]);
    amqp_bytes_t qu_name = amqp_cstring_bytes(argv[4]);
    amqp_bytes_t ex_type = amqp_cstring_bytes("direct");

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
    printf("INFO: step 2-1 : amqp_new_connection() Success\n");

    if ( amqp_ssl_socket_set_cacert(socket, argv[5]) != AMQP_STATUS_OK) {
        printf("ERROR: amqp_ssl_socket_set_cacert() Failed\n");
        exit(EXIT_FAILURE);
    }
    amqp_ssl_socket_set_verify_peer(socket, false);
    amqp_ssl_socket_set_verify_hostname(socket, false);
    printf("INFO: step 2-A : amqp_ssl_socket_set_cacert() Success\n");

    int rc = amqp_socket_open(socket, hostname, port);
    if (rc != AMQP_STATUS_OK) {
        printf("ERROR: amqp_socket_open() Failed\n");
    }

    printf("INFO: step 2-2 : amqp_socket_open() Success\n");

    amqp_rpc_reply_t res = amqp_login(conn, "my_vhost", 0, AMQP_DEFAULT_FRAME_SIZE, 0,
                                      AMQP_SASL_METHOD_PLAIN, "test", "test");
    if ( res.reply_type != AMQP_RESPONSE_NORMAL) {
        printf("ERROR: amqp_login() Failed\n");
    }
    printf("INFO: step 2-3 : amqp_login()  Success\n");

    amqp_channel_t channel_id = 1;
    amqp_channel_open(conn, channel_id);
    res = amqp_get_rpc_reply(conn);
    if (res.reply_type != AMQP_RESPONSE_NORMAL) {
        printf("ERROR: amqp_channel_open() Failed\n");
        exit(EXIT_FAILURE);
    }
    printf("INFO: step 2-4 : amqp_channel_open() Success\n");

    /*
     * Step 3 : declare exchange
     *      1)  amqp_exchange_declare()
     *      2)  amqp_get_rpc_reply()
     */

    amqp_boolean_t passive = 0;
    amqp_boolean_t durable = 1;
    amqp_boolean_t auto_delete = 0;
    amqp_boolean_t internal = 0;
    amqp_boolean_t exclusive = 0;

    amqp_exchange_declare(conn, channel_id, ex_name, ex_type,
                      passive, durable, auto_delete, internal, amqp_empty_table);
    res =amqp_get_rpc_reply(conn);
    if ( res.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION ) {
        printf("ERROR: Library/Socket -> %s\n", amqp_error_string2(res.library_error));
    } else if ( res.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION ) {
        if (res.reply.id == AMQP_CHANNEL_CLOSE_METHOD) {
            amqp_channel_close_t *m = (amqp_channel_close_t *)res.reply.decoded;
            printf("ERROR: Channel -> %s\n", (char *) m->reply_text.bytes);
            exit(EXIT_FAILURE);
        } else if (res.reply.id == AMQP_CONNECTION_CLOSE_METHOD) {
            amqp_connection_close_t *m = (amqp_connection_close_t *)res.reply.decoded;
            printf("ERROR: Connection -> %s\n", (char *) m->reply_text.bytes);
            exit(EXIT_FAILURE);
        }
    } else if (res.reply_type != AMQP_RESPONSE_NORMAL) {
        printf("ERROR: amqp_exchange_declare(%d) Failed\n", res.reply_type);
        exit(EXIT_FAILURE);
    }
    printf("INFO: step 3-1 : amqp_exchange_declare() Success\n");

    /*
     * Step 4 : declare queue (qu_name from argv[4])
     *      1) amqp_simple_rpc(AMQP_QUEUE_DECLARE_METHOD)
     *      2) amqp_release_buffers()
     *      3) amqp_queue_bind()
     */

    amqp_table_t qu_args = amqp_empty_table;
    amqp_queue_declare_t s;
    s.ticket = 0;
    s.queue = qu_name;
    s.passive = passive;
    s.durable = durable;
    s.exclusive = exclusive;
    s.auto_delete = auto_delete;
    s.nowait = 0;
    s.arguments = amqp_empty_table;

    amqp_method_number_t method_ok = AMQP_QUEUE_DECLARE_OK_METHOD;
    res = amqp_simple_rpc(conn, channel_id, AMQP_QUEUE_DECLARE_METHOD, &method_ok , &s);
    amqp_release_buffers(conn);
    amqp_queue_bind(conn, channel_id, qu_name, ex_name, rt_key, qu_args);

    /*
     *  Step 5 : loop input message body and publish
     *           amqp_bytes_t::
     *               size_t amqp_bytes_t::len
     *               void * amqp_bytes_t::bytes
     *           amqp_basic_publish()
     */

    amqp_basic_properties_t hdr_propts;
    hdr_propts._flags  = AMQP_BASIC_CONTENT_TYPE_FLAG;
    hdr_propts._flags |= AMQP_BASIC_CONTENT_ENCODING_FLAG;
    hdr_propts._flags |= AMQP_BASIC_DELIVERY_MODE_FLAG;
    hdr_propts._flags |= AMQP_BASIC_HEADERS_FLAG;
    hdr_propts.content_type = amqp_cstring_bytes("text/plain");
    hdr_propts.content_encoding = amqp_cstring_bytes("UTF-8");
    hdr_propts.delivery_mode = AMQP_DELIVERY_PERSISTENT;
    hdr_propts.headers = amqp_empty_table;

    amqp_boolean_t mandatory = 0;
    amqp_boolean_t immediate = 0;
    std::string msg_body;
    while (1) {

        /*
         *  Step 6 : Input message from console
         */
        std::cout << "Input Message : ";
        msg_body.clear();
        std::getline(std::cin, msg_body);
        if ( msg_body == "end" ) {
            break;
        }

        /*
         *  Step 7 : publish message to RabbitMQ
         */
        amqp_bytes_t msg_body_bytes = amqp_cstring_bytes(msg_body.c_str());
        amqp_basic_publish(conn, channel_id, ex_name, rt_key, mandatory, immediate,
                           &hdr_propts, msg_body_bytes);
    }

    /*
     *  Step 8 : Close connection
     */
    amqp_channel_close(conn, channel_id, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
    return EXIT_SUCCESS;
}
