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

static void my_request_reply(struct evhttp_request *req, void *param)
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

    struct evbuffer *reply = evbuffer_new();     // <-- Allocate storage for a new evbuffer.
    evbuffer_add_printf(reply, "<!DOCTYPE html>\n"
                               "<html lang=\"en\">"
                               "<head>\n"
                               "</head>\n"
                               "<body>\n"
                               "<div style=\"font-size: 36px; font-weight: bold; text-align: center; color: blue;\">\n"
                               "Hui Test Libevent HTTP Server <br>callback : my_request_reply()</br>\n"
                               "<br>Server hostname : %s  : Port (%d)</br>\n"
                               "</div>\n"
                               "</body>\n"
                               "</html>\n", http_server_hostname, (int)http_server_port);
    evhttp_send_reply(req, HTTP_OK, "OK", reply);
    evbuffer_free(reply);
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

    char http_host[] = "0.0.0.0";
    http_server_port = (ev_uint16_t)std::stoi(argv[1]);

    base = event_base_new();           // Create a new event base
    http_server = evhttp_new(base);    // Create a new HTTP server

    evhttp_set_gencb(http_server, my_request_reply, NULL);   // Set a callback for all requests
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
  
