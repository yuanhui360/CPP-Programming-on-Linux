#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <amqp.h>
#include <amqp_tcp_socket.h>

#include <string>
#include <vector>
#include <map>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#define SUMMARY_EVERY_US 1000000

void amqp_usage()
{
    std::cout << "Usage: amqp_publish <host:port> <exchange:type> <rounting key|topic pattern> [queue]" <
    std::cout << "Example :" << std::endl;
    std::cout << "amqp_publish 192.168.0.103:5672 ExchangeName:direct rt_key direct_queue" << std::endl;
    std::cout << "amqp_publish 192.168.0.103:5672 ExchangeName:topic topic|pattern topic_queue" << std::e
    std::cout << "amqp_publish 192.168.0.103:5672 ExchangeName:fanout fanout_queue" << std::endl;
    std::cout << "amqp_publish 192.168.0.103:5672 ExchangeName:headers header_queue" << std::endl;
    exit(EXIT_FAILURE);
}

amqp_table_entry_t*  set_amqp_table_entries(std::map<std::string, std::string> inMap)
{
    if ( inMap.size() == 0 ) { return nullptr; }

    size_t arg_size = sizeof(amqp_table_entry_t) * inMap.size();
    amqp_table_entry_t *ret_entries = (amqp_table_entry_t*) malloc(arg_size);
    int i = 0;
    for (auto it = inMap.begin(); it != inMap.end(); it++ ) {
        ret_entries[i].key = amqp_cstring_bytes(it->first.c_str());
        ret_entries[i].value.kind = AMQP_FIELD_KIND_UTF8;
        ret_entries[i].value.value.bytes = amqp_cstring_bytes(it->second.c_str());
        i++;
    }
    return ret_entries;
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

int main(int argc, char* argv[]) {

    std::vector<std::string> sVec;
    std::vector<std::string> eVec;

    char const *hostname;
    int port;
    amqp_rpc_reply_t res;

    if (argc < 3) { amqp_usage(); }

    /*
     *  Step 1 : parse and validate command line syntax
     *       1) argv[1] format <host:port>
     *       2) argv[2] format <exchange:type>
     *       3) if exchange type is fanout or headers
     *          a) argc == 3 or 4
     *          b) qu_name = argv[3] if argc == 4
     *          c) there is no rt_key
     *       4) if exchange type is not fanout
     *          a) argc = 4 or 5
     *          b) qu_name = argv[4] if argc == 5
     *          c) rt_key = argv[3]
     */

    sVec.clear();
    boost::split(sVec, argv[1], boost::is_any_of(":"));
    if ( sVec.size() != 2 ) { amqp_usage(); }
    hostname = sVec[0].c_str();
    port = std::atoi(sVec[1].c_str());

    eVec.clear();
    boost::split(eVec, argv[2], boost::is_any_of(":"));
    if ( eVec.size() != 2 ) { amqp_usage(); }
    amqp_bytes_t ex_name = amqp_cstring_bytes(eVec[0].c_str());
    amqp_bytes_t ex_type = amqp_cstring_bytes(eVec[1].c_str());

    amqp_bytes_t qu_name = amqp_empty_bytes;
    amqp_bytes_t rt_key  = amqp_empty_bytes;
    if ( eVec[1] == "fanout" || eVec[1] == "headers" ) {
        if ( argc == 4 ) { qu_name = amqp_cstring_bytes(argv[3]); }
        if ( argc > 4 ) { amqp_usage(); }
    }
    else {
        rt_key = amqp_cstring_bytes(argv[3]);
        if ( argc == 5 ) { qu_name = amqp_cstring_bytes(argv[4]); }
        if ( argc > 5 || argc < 4) { amqp_usage(); }
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

    res = amqp_login(conn, "my_vhost", 0, AMQP_DEFAULT_FRAME_SIZE, 0, AMQP_SASL_METHOD_PLAIN, "test", "te
    if ( res.reply_type != AMQP_RESPONSE_NORMAL) {
        exit_with_error(conn, 204);
    }
    amqp_channel_t channel_id = 1;
    amqp_channel_open(conn, channel_id);
    res = amqp_get_rpc_reply(conn);
    if (res.reply_type != AMQP_RESPONSE_NORMAL) {
        exit_with_error(conn, 205);
    }

    /*
     *  Step 3 : if exchange type is headers, input header attributes
     *
     *       std::map<std::string, std::string> hdrMap;
     *       if eVec[1] is headers  {
     *          loop input <key:value> insert into hdrMap
     *          split(hVec, hdrKeyValStr, boost::is_any_of(":"));
     *       }
     *       allocate memory and set amqp_table_entry_t object pointer
     */

    std::map<std::string, std::string> hdrMap;
    hdrMap.clear();
    if ( eVec[1] == "headers" ) {
        std::vector<std::string> hVec;
        std::string  hdrKeyValStr;

        std::cout << "Loop Header (Key:Value) Start: x-match:any or x-match:all, Finish loop: end:end" <<
        while (1) {
            hdrKeyValStr.clear();
            hVec.clear();
            std::cout << "Please input header <key:Value> : ";
            std::getline(std::cin, hdrKeyValStr);
            boost::split(hVec, hdrKeyValStr, boost::is_any_of(":"));

            if (hVec.size() != 2) { continue; }
            if (hVec[0] == "end" && hVec[1] == "end") { break; }
            hdrMap[hVec[0]] = hVec[1];
        }
    }

    amqp_table_entry_t* hdr_entries = set_amqp_table_entries(hdrMap);

    /*
     * Step 4 : declare exchange
     *      1)  amqp_exchange_declare()
     *      2)  amqp_get_rpc_reply()
     */

    amqp_boolean_t passive = 0;
    amqp_boolean_t durable = 1;
    amqp_boolean_t auto_delete = 1;
    amqp_boolean_t internal = 0;
    amqp_boolean_t exclusive = 0;

    amqp_exchange_declare(conn, channel_id, ex_name, ex_type,
                      passive, durable, auto_delete, internal, amqp_empty_table);
    res =amqp_get_rpc_reply(conn);
    if (res.reply_type != AMQP_RESPONSE_NORMAL) {
        exit_with_error(conn, 400);
    }

    /*
     * Step 5 : declare queue if qu_name provided
     *      1) amqp_simple_rpc(AMQP_QUEUE_DECLARE_METHOD)
     *      2) amqp_release_buffers()
     *      3) amqp_queue_bind()
     */

    if ( qu_name.len > 0) {
        durable = 0;
        auto_delete = 0;

        /*
         *   amqp_table_t ex_args;
         *   ex_args.num_entries = 0; <- int
         *   ex_args.entries = NULL;  <- amqp_table_entry_t*
         */

        amqp_table_t qu_args;
        qu_args.num_entries = hdrMap.size();
        qu_args.entries = hdr_entries;

        amqp_queue_declare_t s;
        s.ticket = 0;
        s.queue = qu_name;
        s.passive = passive;
        s.durable = durable;
        s.exclusive = exclusive;
        s.auto_delete = auto_delete;
        s.nowait = 0;
        s.arguments = qu_args;

        amqp_method_number_t method_ok = AMQP_QUEUE_DECLARE_OK_METHOD;
        res = amqp_simple_rpc(conn, channel_id, AMQP_QUEUE_DECLARE_METHOD, &method_ok , &s);
        amqp_release_buffers(conn);
        amqp_queue_bind(conn, channel_id, qu_name, ex_name, rt_key, qu_args);
    }

    /*
     *  Step 6 : steup basic message properties
     */

    amqp_basic_properties_t hdr_propts;
    hdr_propts._flags  = AMQP_BASIC_CONTENT_TYPE_FLAG;
    hdr_propts._flags |= AMQP_BASIC_CONTENT_ENCODING_FLAG;
    hdr_propts._flags |= AMQP_BASIC_DELIVERY_MODE_FLAG;
    hdr_propts._flags |= AMQP_BASIC_HEADERS_FLAG;
    hdr_propts.content_type = amqp_cstring_bytes("text/plain");
    hdr_propts.content_encoding = amqp_cstring_bytes("UTF-8");
    hdr_propts.delivery_mode = AMQP_DELIVERY_PERSISTENT;
    hdr_propts.headers.num_entries = hdrMap.size();
    hdr_propts.headers.entries = hdr_entries;

    amqp_boolean_t mandatory = 0;
    amqp_boolean_t immediate = 0;

    /*
     *  Step 7 : loop input message body and publish
     *           amqp_bytes_t::
     *               size_t amqp_bytes_t::len
     *               void * amqp_bytes_t::bytes
     *
     *           amqp_basic_publish()
     */

    std::string msg_body;
    while (1) {
        std::cout << "Please input Message : ";
        msg_body.clear();
        std::getline(std::cin, msg_body);
        if ( msg_body == "end" ) {
            break;
        }
        amqp_bytes_t msg_body_bytes = amqp_cstring_bytes(msg_body.c_str());
        amqp_basic_publish(conn, channel_id, ex_name, rt_key, mandatory, immediate,
                           &hdr_propts, msg_body_bytes);
    }

    if ( hdr_entries ) { free(hdr_entries); }
    amqp_channel_close(conn, channel_id, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
    return EXIT_SUCCESS;
}
