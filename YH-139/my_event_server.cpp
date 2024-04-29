#include <netinet/in.h>          /* For sockaddr_in */
#include <sys/socket.h>          /* For socket functions */
#include <fcntl.h>               /* For fcntl */

#include <event2/event.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string>

#define MAX_LINE 16384
struct my_event_param {
    char buffer[MAX_LINE];
    size_t buffer_used;
    struct event_base *base;
    struct event *read_event;
    struct event *write_event;
};

void do_read(evutil_socket_t fd, short events, void *arg);
void do_write(evutil_socket_t fd, short events, void *arg);
void do_accept(evutil_socket_t listener, short event, void *arg);

void do_read(evutil_socket_t fd, short events, void *arg)
{
    /*
     *  1) decoding argument
     *  2) memset msg buffer                : memset()
     *  3) receive a message from a socket  : recv()
     *  4) if msg, create write event       : event_new() callback do_write()
     *  5) add write even to base           : event_add()
     *  6) no recvd msg, free memory        : event_free(), free()
     */

    struct my_event_param *ev_ptr = (struct my_event_param*) arg;
    char *ptr = ev_ptr->buffer;       // <-- declare pointer pint to the beginner of buffer
    memset(ptr, '\0', MAX_LINE);
    ssize_t recvd_length = recv(fd, ptr, MAX_LINE, 0);
    if ( recvd_length <= 0 ) {
        event_free(ev_ptr->read_event);
        event_free(ev_ptr->write_event);
        free(ev_ptr);
    } else {
        ev_ptr->buffer_used = recvd_length;
        printf("do_read event : <%d> <-- %s\n", (int)recvd_length, ev_ptr->buffer);
        ev_ptr->write_event = event_new(ev_ptr->base, fd, EV_WRITE|EV_PERSIST, do_write, ev_ptr);
        event_add(ev_ptr->write_event, NULL);
    }
}

void do_write(evutil_socket_t fd, short events, void *arg)
{
    /*
     *   1) send message back
     *   2) Remove write_event from base
     */

    struct my_event_param *ev_ptr = (struct my_event_param*) arg;
    char *ptr = ev_ptr->buffer;    // <-- point to the beginning of buffer
    unsigned int remaining = ev_ptr->buffer_used;
    ssize_t byte_sent = 0;
    printf("do_write event : <%d> <-- %s\n", (int)strlen(ev_ptr->buffer), ev_ptr->buffer);
    while (remaining > 0 ) {
        byte_sent = send(fd, ptr, remaining, 0);
        if ( byte_sent <=0 )
            break;
        remaining -= byte_sent;
        ptr += byte_sent;
    }
    event_del(ev_ptr->write_event);
}

void do_accept(evutil_socket_t listener, short event, void *arg)
{
    /*
     *   1) decoding argument cast : struct event_base
     *   2) declare socked address : struct sockaddr_storage ss;
     *   3) accept connection on socket and return fd  : accept()
     *   If success:
     *      4) set socket nonblocking                     : evutil_make_socket_nonblocking()
     *      5) Allocate memory for my event parameter     : malloc()
     *      6) Initialize my_event_parameter
     *      7) create read event callback do_read()       : event_new()
     *      8) add read event to event base               : event_add()
     */

    struct event_base *base = (struct event_base *)arg;
    struct sockaddr_storage  ss;    // <-- A struct at lease as alrge as any other sockaddr_*
    socklen_t slen = sizeof(ss);
    int fd = accept(listener, (struct sockaddr*)&ss, &slen);
    if ( fd < 0 ) {
        printf("accept erro, listener : <%d>\n", listener);
    } else if ( fd > FD_SETSIZE ) {
        close(fd);
    } else {
        evutil_make_socket_nonblocking(fd);
        struct my_event_param *ev_ptr = (struct my_event_param*) malloc(sizeof(struct my_event_param));
        memset(ev_ptr, '\0', sizeof(struct my_event_param));
        ev_ptr->buffer_used = 0;
        ev_ptr->base = base;
        ev_ptr->read_event = NULL;
        ev_ptr->write_event = NULL;

        ev_ptr->read_event = event_new(base, fd, EV_READ|EV_PERSIST, do_read, ev_ptr);
        event_add(ev_ptr->read_event, NULL);
    }
}

void run(int port_num)
{
    /*
     *  1) assign socket address          <-- struct sockaddr_in sin;
     *  2) Create new Socket (listener)   <-- socket()
     *  3) bind a name to socket          <-- bind()
     *  4) listen for connections         <-- listen()
     *  5) Creating an event base         <-- event_base_new()
     *                                        event_base_new_with_config().
     *  6) Make socket non block          <-- evutil_make_socket_nonblocking()
     *  7) new listener Event add to base <-- event_new() and event_add()
     *  8) Dispatching events             <-- event_base_dispatch()
     *  evutil_socket_t listener;             // /usr/include/event2/util.h
     */

    struct sockaddr_in    sin;
    sin.sin_family        = AF_INET;       // <-- IPv4 internet protcol
    sin.sin_addr.s_addr   = INADDR_ANY;    // <-- Accept any incoming messages (0)
    sin.sin_port          = htons(port_num);

    evutil_socket_t listener = socket(AF_INET, SOCK_STREAM, 0);
    int rc = bind(listener, (struct sockaddr*) &sin, sizeof(sin));
    if ( rc < 0 ) {
        printf("bind socket error, port number : <%d>, listener : %d\n", port_num, listener);
        return;
    }

    rc = listen(listener, 16);
    if ( rc < 0 ) {
        printf("listen for connection error, listener : <%d>\n", listener);
        return;
    }

    struct event_base *base = event_base_new();    // <-- Create an event base
    if ( !base ) {
        printf("Creating an event base error\n");
        return;
    }

    evutil_make_socket_nonblocking(listener);
    struct event *listener_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept, (void*)base);
    event_add(listener_event, NULL);
    event_base_dispatch(base);
}

int main(int argc, char *argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);
    if ( argc != 2 ) {
        printf("usage: %s <port>\n", argv[0]);
        printf("Example : %s 1234\n", argv[0]);
        return 1;
    }

    int port_num = std::stoi(argv[1]);
    run(port_num);
    return 0;
}
