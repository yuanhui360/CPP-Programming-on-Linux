#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <evhttp.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/bufferevent.h>

#define MAX_SIZE      1024

void http_request_cb(struct evhttp_request *req, void *arg){

    /*
     *   1) allocate memory for reading respond from https server
     *   2) make loop read from request input buffer and print data
     *   3) event_base_loopbreak()
     */

    char buf[MAX_SIZE];
    memset(buf, '\0', MAX_SIZE);

    while ( evbuffer_remove(req->input_buffer, &buf, MAX_SIZE) > 0 ) {
        printf("%s", buf);
        memset(buf, '\0', MAX_SIZE);
    }
    event_base_loopbreak((struct event_base *)arg);
}

int main(int argc, char *argv[]){

    struct event_base *base = nullptr;
    struct evhttp_connection *conn = nullptr;
    struct evhttp_request *req = nullptr;

    if ( argc != 2 ) {
        printf("Usgae : %s <uri>\n", argv[0]);
        printf("uri pattern:\n");
        printf("  scheme://[[userinfo]@]foo.com[:port]]/[path][?query][#fragment]\n");
        printf("Example:\n");
        printf("  %s http://10.0.0.190:1234/\n", argv[0]);
        return -1;
    }

    struct evhttp_uri *http_uri = evhttp_uri_parse(argv[1]);
    const char *http_scheme = evhttp_uri_get_scheme(http_uri);
    const char *http_host = evhttp_uri_get_host(http_uri);
    int http_port = evhttp_uri_get_port(http_uri);
    printf("HTTP request: %s::/%s:%d\n", http_scheme, http_host, http_port);

    base = event_base_new();         // Create event base object
    conn = evhttp_connection_base_new(base, NULL, http_host, http_port); // create connection object
    req = evhttp_request_new(http_request_cb, base);

    evhttp_add_header(req->output_headers, "Host", http_host);
    evhttp_add_header(req->output_headers, "Connection", "close");

    evhttp_make_request(conn, req, EVHTTP_REQ_GET, "/");
    evhttp_connection_set_timeout(req->evcon, 600);
    event_base_dispatch(base);
    return 0;
}
