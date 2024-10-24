#include <iostream>   // std::cout
#include <string>     // std::string, std::stoi
#include <unistd.h>   // gethostname
#include <sstream>
#include <vector>
#include <string.h>
#include <signal.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/keyvalq_struct.h>

/*
 * /usr/include/event2/http.h
 *
 * const struct evhttp_uri *parsed_uri = evhttp_request_get_evhttp_uri(req);
 *       struct evhttp_uri *parsed_uri = evhttp_uri_parse(request_uri);
 * const char *request_fragment = evhttp_uri_get_fragment(parsed_uri);
 */

char http_server_hostname[255];
static ev_uint16_t http_server_port;

static void connect_to_recv_cb(struct evhttp_request *req, void *param)
{
    // This callback funct will process received destination HTTP server response
    // and then returning it back to the client
    printf("my_request_proxy()->connect_to_recv_cb() Invoked\n");
    ev_ssize_t len;
    struct evhttp_request *client_req = (struct evhttp_request *) param;
    struct evbuffer *reply = evbuffer_new();
    struct evbuffer *connect_to_evbuf = evhttp_request_get_input_buffer(req);
    len = evbuffer_get_length(connect_to_evbuf);

    fwrite(evbuffer_pullup(connect_to_evbuf, len), len, 1, stdout);
    evbuffer_add_buffer(reply, connect_to_evbuf);
    evbuffer_drain(connect_to_evbuf, len);

    evbuffer_add_buffer(reply, connect_to_evbuf);
    evhttp_send_reply(client_req, HTTP_OK, "OK", reply);
    evbuffer_free(reply);

}
static void my_request_proxy(struct evhttp_request *req, void *param)
{
    const char *request_uri = evhttp_request_get_uri(req);
    const char *request_host = evhttp_request_get_host(req);
    enum evhttp_cmd_type cmd = evhttp_request_get_command(req);
    const struct evhttp_uri *parsed_uri = evhttp_request_get_evhttp_uri(req);
    const char *request_schema = evhttp_uri_get_scheme(parsed_uri);
    int  request_port = evhttp_uri_get_port(parsed_uri);
    const char *request_path = evhttp_uri_get_path(parsed_uri);
    const char *request_query = evhttp_uri_get_query(parsed_uri);
    const char *cmd_type;
    switch (cmd) {
        case EVHTTP_REQ_GET: cmd_type = "GET"; break;
        case EVHTTP_REQ_POST: cmd_type = "POST"; break;
        case EVHTTP_REQ_HEAD: cmd_type = "HEAD"; break;
        case EVHTTP_REQ_PUT: cmd_type = "PUT"; break;
        case EVHTTP_REQ_DELETE: cmd_type = "DELETE"; break;
        case EVHTTP_REQ_OPTIONS: cmd_type = "OPTIONS"; break;
        case EVHTTP_REQ_TRACE: cmd_type = "TRACE"; break;
        case EVHTTP_REQ_CONNECT: cmd_type = "CONNECT"; break;
        case EVHTTP_REQ_PATCH: cmd_type = "PATCH"; break;
        default: cmd_type = "unknown"; break;
    }

    printf("my_request_reply() request uri : <%s> : Request Type <%s> : Schema <%s> : Host <%s> : Port (%
           request_uri, cmd_type, request_schema, request_host, request_port);
    printf("my_request_reply() Path : <%s> Query : <%s>\n", request_path, request_query);

    // starting processing forward client HTTP request to destination server
    std::stringstream         ss;
    if (request_host == nullptr ) {
        printf("forward request host missing : <%s>\n",request_host);
        evhttp_send_error(req,HTTP_BADREQUEST, 0);
        return;
    }
    if ( request_host == nullptr || request_port <= 0 ) {
        // rebuild connect to uri
        if ( request_port <= 0 ) {
            request_port = 80;       // default port number for HTTP
        }
        ss << "http://" << request_host << ":" << request_port;
    } else {
        ss << request_uri;
    }
    /*
     *  4 major steps to make client request to the destination HTTP server
     *  1) initialization evhttp_connection object
     *  2) create evhttp_request object
     *  3) pass client headers, bodys data to new request
     *  4) make request
     */

    struct evhttp_connection *client_conn = evhttp_request_get_connection(req);
    if ( client_conn == nullptr ) {
        printf("evhttp_request_get_connection(req) Error\n");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }
    struct event_base *base = evhttp_connection_get_base(client_conn);
    if ( base == nullptr ) {
        printf("evhttp_connection_get_base(client_conn) Error\n");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }

    struct evhttp_connection *connect_to_conn = evhttp_connection_base_new(base, NULL, request_host, requ
    if ( connect_to_conn == nullptr ) {
        printf("evhttp_connection_base_new(connect_to_conn) Error\n");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }
    struct evhttp_request *connect_to_req = evhttp_request_new(connect_to_recv_cb, req);
    if ( connect_to_req == nullptr ) {
        printf("evhttp_request_new(connect_to_req) error\n");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }

    // pass headers data
    struct evkeyvalq *output_headers = evhttp_request_get_output_headers(connect_to_req);
    struct evkeyvalq *input_headers = nullptr;
    struct evkeyval *input_header = nullptr;
    input_headers = evhttp_request_get_input_headers(req);
    for ( input_header = input_headers->tqh_first; input_header;
          input_header = input_header->next.tqe_next) {
        // loop pass original input headers to connect to req output headers
        evhttp_add_header(output_headers, input_header->key, input_header->value);
    }

    // pass body data
    evbuffer_add_buffer(evhttp_request_get_output_buffer(connect_to_req),
                        evhttp_request_get_input_buffer(req));
    evhttp_make_request(connect_to_conn, connect_to_req, cmd, ss.str().c_str());

}

static void my_signal_event_cb(evutil_socket_t fd, short event, void *arg)
{
    struct event_base *base = (struct event_base *)arg;
    printf("%s signal received\n", strsignal(fd));
    event_base_loopbreak(base);
}

int main(int argc, char *argv[])
{

    static struct event_base *base;
    static struct evhttp *http_server;

    if ( argc < 2 ) {
        printf("usage: %s <port>\n", argv[0]);
        printf("Example : %s 1234\n", argv[0]);
        return -1;
    }

    memset(http_server_hostname, '\0', sizeof(http_server_hostname));
    gethostname(http_server_hostname, sizeof(http_server_hostname));

    char http_host[] = "0.0.0.0";       // INADDR_ANY
    http_server_port = (ev_uint16_t)std::stoi(argv[1]);

    base = event_base_new();           // Create a new event base
    http_server = evhttp_new(base);    // Create a new HTTP server

    evhttp_set_gencb(http_server, my_request_proxy, NULL);   // Set a callback for all requests
    int http_handle = evhttp_bind_socket(http_server, (const char *)http_host, http_server_port);

    struct event *sig_int = evsignal_new(base, SIGINT, my_signal_event_cb, base);
    event_add(sig_int, NULL);

    printf("Listening requests uri on http://%s:%d (%d)\n", http_host, http_server_port, http_handle);
    printf("uri pattern:\n");
    printf("  scheme://[[userinfo]@]foo.com[:port]]/[path][?query][#fragment]\n");
    printf("Request Example:\n");
    printf("     http://10.0.0.191:%d/\n", (int)http_server_port);

    event_base_dispatch(base);

    evhttp_free(http_server);
    event_free(sig_int);
    event_base_free(base);
}
