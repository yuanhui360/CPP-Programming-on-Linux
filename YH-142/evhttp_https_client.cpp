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

#include <event2/bufferevent_ssl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAX_SIZE      1024

void https_request_cb(struct evhttp_request *req, void *arg){
    char buf[MAX_SIZE];
    memset(buf, '\0', MAX_SIZE);

    while ( evbuffer_remove(req->input_buffer, &buf, MAX_SIZE) > 0 ) {
        printf("%s", buf);
        memset(buf, '\0', MAX_SIZE);
    }
    event_base_loopbreak((struct event_base *)arg);
}

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
    struct bufferevent *bev = nullptr;

    if ( argc != 2 ) {
        printf("Usgae : %s <uri>\n", argv[0]);
        printf("uri pattern:\n");
        printf("  scheme://[[userinfo]@]foo.com[:port]]/[path][?query][#fragment]\n");
        printf("Example:\n");
        printf("  %s https://10.0.0.190:1234/\n", argv[0]);
        return -1;
    }

    struct evhttp_uri *http_uri = evhttp_uri_parse(argv[1]);
    const char *http_scheme = evhttp_uri_get_scheme(http_uri);
    const char *http_host = evhttp_uri_get_host(http_uri);
    int http_port = evhttp_uri_get_port(http_uri);
    printf("HTTP request: %s::/%s:%d\n", http_scheme, http_host, http_port);

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());  // <-- Creat SSL Context for client
    if ( ctx == nullptr ) {
        printf("Create SSL Context error\n");
        exit(-1);
    }
    SSL *my_ssl = SSL_new(ctx); // <-- Create SSL object
    if ( my_ssl == nullptr ) {
        printf("Create SSL Object error\n");
        exit(-1);
    }

    base = event_base_new();         // Create event base object
    if ( strcasecmp(http_scheme, "http") == 0 ) {
        // DO HTTP request
        conn = evhttp_connection_base_new(base, NULL, http_host, http_port); // create connection object
        req = evhttp_request_new(http_request_cb, base);
    }
    else if ( strcasecmp(http_scheme, "https") == 0 ) {
        // DO HTTPS request
        bev = bufferevent_openssl_socket_new(base, -1, my_ssl, BUFFEREVENT_SSL_CONNECTING,
                     BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
        if ( bev == nullptr ) {
            printf("bufferevent_openssl_socket_new() failed\n");
            exit(-1);
        }
        conn = evhttp_connection_base_bufferevent_new(base, NULL, bev, http_host, http_port);
        req = evhttp_request_new(https_request_cb, base);
    }
    else {
        printf("shceme not supported (%s)\n", http_scheme);
        exit(-1);
    }

    evhttp_add_header(req->output_headers, "Host", http_host);
    evhttp_add_header(req->output_headers, "Connection", "close");

    evhttp_make_request(conn, req, EVHTTP_REQ_GET, "/");
    evhttp_connection_set_timeout(req->evcon, 600);
    event_base_dispatch(base);

    SSL_shutdown(my_ssl);
    SSL_free(my_ssl);
    SSL_CTX_free(ctx);
    return 0;
}
