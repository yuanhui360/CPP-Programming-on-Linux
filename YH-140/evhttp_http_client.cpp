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

    struct event_base *base;
    struct evhttp_connection *conn;
    struct evhttp_request *req;

    if ( argc != 3 ) {
        printf("Usgae : %s <host> <port>\n", argv[0]);
        printf("Example:\n");
        printf("  %s 10.0.0.191 1234\n", argv[0]);
        return -1;
    }

    /*
     *  1) header file  <evhttp.h> <event2/event.h>
     *                  <event2/http.h> <event2/bufferevent.h>
     *  2) Create a new event_base       : event_base_new()
     *  3) Create connection object      : evhttp_connection_base_new()
     *  4) Creates a new request object  : evhttp_request_new()
     *  5) Set request header            : evhttp_add_header()
     *  6) Make an HTTP request          : evhttp_make_request()
     *  7) set connection timeout        : evhttp_connection_set_timeout()
     *  8) dispatch action event base    : event_base_dispatch()
     */

    const char * req_host = argv[1];
    int req_port = std::stoi(argv[2]);

    base = event_base_new();         // Create event base object
    conn = evhttp_connection_base_new(base, NULL, req_host, req_port); // create connection object
    req = evhttp_request_new(http_request_cb, base);

    evhttp_add_header(req->output_headers, "Host", req_host);
    evhttp_add_header(req->output_headers, "Connection", "close");

    evhttp_make_request(conn, req, EVHTTP_REQ_GET, "/");
    evhttp_connection_set_timeout(req->evcon, 600);
    event_base_dispatch(base);
    return 0;
}
