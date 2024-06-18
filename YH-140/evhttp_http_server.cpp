#include <iostream>   // std::cout
#include <string>     // std::string, std::stoi

#include <string.h>
#include <signal.h>
#include <evhttp.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>

/*
 * /usr/include/event2/http.h
 */

static void my_request_reply(struct evhttp_request *req, void *ctx)
{
    /*
     *   1) Allocate ev_buffer for reply
     *   2) GET and Parse request URI (cmd type, host)
     *   3) write HTML reply  to buffer
     *   4) send HTTP reply
     *   5) free allocated memory
     */
    struct evbuffer *reply = evbuffer_new();     // <-- Allocate storage for a new evbuffer.

    const char *request_uri = evhttp_request_get_uri(req);
    const char *request_host = evhttp_request_get_host(req);
    struct evhttp_uri *parsed_uri = evhttp_uri_parse(request_uri);
    if (!parsed_uri) {
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }

    enum evhttp_cmd_type cmd = evhttp_request_get_command(req);
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
    printf("request  <%s> received, cmd type <%s> host : %s\n", request_uri, cmd_type, request_host);

    /*
     "<!DOCTYPE html>\n"
     "<html lang=\"en\">"
     "<head>\n"
     "</head>\n"
     "<body>\n"
     "<div style=\"font-size: 36px; font-weight: bold; text-align: center; color: blue;\">\n"
     "Hui Test Libevent HTTP Server\n"
     "</div>\n"
     "</body>\n"
     "</html>\n");
     */
    
    evbuffer_add_printf(reply, "<!DOCTYPE html>\n"
                               "<html lang=\"en\">"
                               "<head>\n"
                               "</head>\n"
                               "<body>\n"
                               "<div style=\"font-size: 36px; font-weight: bold; text-align: center; color: blue;\">\n"
                               "Hui Test Libevent HTTP Server\n"
                               "</div>\n"
                               "</body>\n"
                               "</html>\n");

    evhttp_send_reply(req, HTTP_OK, NULL, reply);

    if ( parsed_uri ) {
        evhttp_uri_free(parsed_uri);
    }
    evbuffer_free(reply);
}

static void my_signal_event_cb(evutil_socket_t fd, short event, void *arg)
{
    /*
     *  Signal event SIGINT call back function
     *  SIGINT <-- Interrupt from keyboard
     *  1) Parse, decoding argument
     *  2) event_base_loopbreak()
     */
    struct event_base *base = (struct event_base *) arg;
    printf("%s signal received\n", strsignal(fd));
    event_base_loopbreak(base);
}

int main(int argc, char *argv[])
{

    if ( argc != 2 ) {
        printf("usage: %s <port>\n", argv[0]);
        printf("Example : %s 1234\n", argv[0]);
        return 1;
    }

    /*
     *    Header file : /sur/include/http.h
     *
     *    1) Create a new event base           : function event_base_new()
     *    2) Create a new HTTP server          : function evhttp_new()
     *    3) Binds an HTTP server on the       : function evhttp_bind_socket()
     *       specified address and port.
     *    4) Set a callback for all requests   : function evhttp_set_gencb()
     *       that are not caught by specific
     *       callbacks
     *    5) Create signal event ctrl+C exit   : function evsignal_new()
     *    6) Add signal event to base          : function event_add()
     *    7) Make an event active              : function event_base_dispatch()
     *
     *  End :
     *    8) free http server                  : function evhttp_free()
     *    9) free signal event                 : function event_free()
     *   10) free event base                   : function event_base_free()
     */

     char http_addr[] = "0.0.0.0";
     ev_uint16_t http_port = (ev_uint16_t) std::stoi(argv[1]);

     struct event_base *base = event_base_new();            // Create a new event base
     struct evhttp *http_server = evhttp_new(base);         // Create a new HTTP server

     int http_handle = evhttp_bind_socket(http_server, (const char *) http_addr, http_port);
     evhttp_set_gencb(http_server, my_request_reply, NULL);  // Set callback for all requests

     struct event *sig_int = evsignal_new(base, SIGINT, my_signal_event_cb, base);
     event_add(sig_int, NULL);

     printf("Listening request on http://%s:%d (%d)\n", http_addr, http_port, http_handle);
     event_base_dispatch(base);

     evhttp_free(http_server);
     event_free(sig_int);
     event_base_free(base);
     return 0;

}
