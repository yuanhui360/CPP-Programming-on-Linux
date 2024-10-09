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

#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>
#define BUFFER_SIZE     1024

static struct sockaddr_storage listen_on_addr;
static struct sockaddr_storage connect_to_addr;
static int listen_on_addrlen;
static int connect_to_addrlen;

static void read_cb(struct bufferevent *bev, void *arg)
{
    // Read from buffer (bev) and echo back (bev)
    struct bufferevent *partner = (struct bufferevent *) arg;
    char recv_buf[BUFFER_SIZE];
    memset(recv_buf, '\0', BUFFER_SIZE);

    struct evbuffer *src = bufferevent_get_input(bev);
    size_t len = evbuffer_get_length(src);
    ev_ssize_t n = evbuffer_copyout(src,recv_buf, len);
    printf("Message Recv (%d) - %s\n", (int)n, recv_buf);

    if (!partner) {
        evbuffer_drain(src, len);
        return;
    }

    struct evbuffer *dst = bufferevent_get_output(partner);
    evbuffer_add_buffer(dst, src);    // <-- Move all data from one buffer into aonter buffer
}

static void ending_write_cb(struct bufferevent *bev, void *arg)
{
    struct evbuffer *b = bufferevent_get_output(bev);
    printf("ending_write_cb() Invoked\n");

    if (evbuffer_get_length(b) == 0 ) {
        bufferevent_free(bev);
    }
}

static void event_cb(struct bufferevent *bev, short event, void *arg)
{
    struct bufferevent *partner = (struct bufferevent *) arg;
    printf("event_cb() Invoked\n");
    if ( event & (BEV_EVENT_EOF|BEV_EVENT_ERROR)) {
        if (partner) {
            /* Flush all pending data */
            read_cb(bev, arg);

            if (evbuffer_get_length(bufferevent_get_output(partner))) {
                /* Flush pending data from other side */
                bufferevent_setcb(partner, NULL, ending_write_cb, event_cb, NULL);
                bufferevent_disable(partner, EV_READ);
            } else {
                bufferevent_free(partner);
            }
        }
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
    struct bufferevent *b_in = bufferevent_socket_new(base, fd,
           BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);
    struct bufferevent *b_out = bufferevent_socket_new(base, -1,
           BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);
    assert(b_in && b_out);

    if (bufferevent_socket_connect(b_out, (struct sockaddr*)&connect_to_addr, connect_to_addrlen)<0) {
        printf("bufferevent_socket_connect(connect_to_addr) Error\n");
        bufferevent_free(b_out);
        bufferevent_free(b_in);
        return;
    }

    bufferevent_setcb(b_in, read_cb, NULL, event_cb, b_out);
    bufferevent_setcb(b_out, read_cb, NULL, event_cb, b_in);
    bufferevent_enable(b_in, EV_READ|EV_WRITE);
    bufferevent_enable(b_out, EV_READ|EV_WRITE);
}

int main(int argc, char *argv[])
{

    int rc;

    if ( argc != 3 ) {
        printf("usage: %s <listen_on_addr> <connect_to_addr>\n", argv[0]);
        printf("Example : %s 10.0.0.191:1234 10.0.0.191:4567\n", argv[0]);
        printf("        : %s 1234 10.0.0.191:4567\n", argv[0]);
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

    memset(&listen_on_addr, 0, sizeof(listen_on_addr));
    listen_on_addrlen = sizeof(listen_on_addr);
    rc = evutil_parse_sockaddr_port(argv[1],
             (struct sockaddr*)&listen_on_addr, &listen_on_addrlen);   // 0 on success, -1 error
    if ( rc < 0 ) {
        printf("Info : Parse listen_on_addr() <%s>\n", argv[1]);
        int p = atoi(argv[1]);
        struct sockaddr_in *sin = (struct sockaddr_in*)&listen_on_addr;
        sin->sin_port = htons(p);
        sin->sin_addr.s_addr = INADDR_ANY;
        sin->sin_family = AF_INET;
        listen_on_addrlen = sizeof(struct sockaddr_in);
    }

    memset(&connect_to_addr, 0, sizeof(connect_to_addr));
    connect_to_addrlen = sizeof(connect_to_addr);
    rc = evutil_parse_sockaddr_port(argv[2],
             (struct sockaddr*)&connect_to_addr, &connect_to_addrlen);   // 0 on success, -1 error
    if ( rc < 0 ) {
        printf("Info : Parse connect_to_addr() <%s>\n", argv[1]);
        exit(-1);
    }

    static struct event_base *base;
    base = event_base_new();
    if (!base) {
        printf("event_base_new() Error\n");
        return -1;
    }

    struct evconnlistener *listener = evconnlistener_new_bind(base, accept_cb, NULL,
           LEV_OPT_CLOSE_ON_FREE|LEV_OPT_CLOSE_ON_EXEC|LEV_OPT_REUSEABLE, -1,
           (struct sockaddr*)&listen_on_addr, listen_on_addrlen);
    if ( ! listener ) {
        printf("evconnlistener_new_bind() Error\n");
    }
    evconnlistener_set_error_cb(listener, accept_error_cb);
    printf("Start Listening : %s => %s\n", argv[1], argv[2]);
    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_base_free(base);
    return 0;
}
