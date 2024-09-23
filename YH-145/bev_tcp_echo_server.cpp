#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <event2/event.h>
#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/http.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>
#define BUFFER_SIZE     1024

static void read_cb(struct bufferevent *bev, void *arg)
{
    // Read from buffer (bev) and echo back (bev)
    struct evbuffer *src = bufferevent_get_input(bev);
    struct evbuffer *dst = bufferevent_get_output(bev);
    evbuffer_add_buffer(dst, src);    // <-- Move all data from one buffer into aonter buffer

}

static void event_cb(struct bufferevent *bev, short event, void *arg)
{
    printf("event_cb() Invoked\n");
    if ( event & BEV_EVENT_CONNECTED) {
        printf("event_cb() BEV_EVENT_CONNECTED\n");
    } else if ( event & BEV_EVENT_EOF) {
        printf("event_cb() BEV_EVENT_EOF\n");
        bufferevent_free(bev);
    } else if ( event & BEV_EVENT_ERROR) {
        printf("event_cb() BEV_EVENT_ERROR\n");
        bufferevent_free(bev);
    }
}

static void accept_error_cb(struct evconnlistener *listener, void *ctx)
{
        /*
         *  print error message and exit event base loop
         */
        struct event_base *base = evconnlistener_get_base(listener);
        int err = EVUTIL_SOCKET_ERROR();
        printf("Got an error %d (%s) on the listener. "
               "Shutting doen. \n", err, evutil_socket_error_to_string(err));
        event_base_loopexit(base, NULL);
}

static void accept_cb(struct evconnlistener *listener, evutil_socket_t fd,
                      struct sockaddr *sock_addr, int sock_len, void *param)
{

    /*
     *     1) Accept Listener and creat socket based bufferevent
     *     2) Set bufferevent call back, read_cb, write_cb, event_cb
     *     3) Enable bufferevent (bev) RAED and WRITE
     */

    printf("accept_cb() Invoked\n");
    struct event_base *base = evconnlistener_get_base(listener);
    struct bufferevent *bev = bufferevent_socket_new(base, fd,
           BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);

    bufferevent_setcb(bev, read_cb, NULL, event_cb, NULL);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
}

int main(int argc, char *argv[])
{

    if ( argc != 2 ) {
        printf("usage: %s <port>\n", argv[0]);
        printf("Example : %s 1234\n", argv[0]);
        exit(-1);
    }

    /*
     *   1) Get port number and parse listen address
     *   2) Create Event base
     *   3) Allocate a new evconnlistener object to listen for incoming
     *      TCP connections on a given address (accept_callback)
     *   4) Set accetp error callback
     *   5) dispatch event loop
     *   6) adjust callbacks, read_cb, accept_cb, event_cb or write_cb
     */

    unsigned int port_num = std::stoi(argv[1]);

    static struct sockaddr_in listen_on_addr;
    int listen_slen;
    listen_slen = sizeof(listen_on_addr);
    memset(&listen_on_addr, '\0', listen_slen);

    listen_on_addr.sin_family = AF_INET;
    listen_on_addr.sin_addr.s_addr = INADDR_ANY;
    listen_on_addr.sin_port = htons(port_num);

    static struct event_base *base;
    base = event_base_new();
    if (!base) {
        printf("event_base_new() Error\n");
        return -1;
    }

    struct evconnlistener *listener = evconnlistener_new_bind(base, accept_cb, NULL,
           LEV_OPT_CLOSE_ON_FREE|LEV_OPT_CLOSE_ON_EXEC|LEV_OPT_REUSEABLE, -1,
           (struct sockaddr*)&listen_on_addr, listen_slen);
    if ( ! listener ) {
        printf("evconnlistener_new_bind() Error\n");
    }
    evconnlistener_set_error_cb(listener, accept_error_cb);
    printf("Start Listening : %s\n", argv[1]);
    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_base_free(base);
    return 0;
}
