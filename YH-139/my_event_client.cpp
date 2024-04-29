#include <netinet/in.h>         /* <--  For sockaddr_in  */
#include <sys/socket.h>         /* <--  For socket functions */
#include <netdb.h>              /* <--  For gethostbyname */

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <sstream>

#include <event2/event.h>

const int max_length = 1024;

char msg_buf[max_length];
int do_read_count;
int do_write_count;

void do_read(evutil_socket_t fd, short events, void *arg)
{
    struct event_base *base = (struct event_base *)arg;
    char recv_buf[max_length];

    /*
     *    char recv_buf[max_length];
     *
     *    1) set fd non blocking
     *    2) allocate and memset receiving buffer
     *    3) receiving data from socket   function recv()
     *    4) event loop continue, break, exit
     *       event_base_loopbreak(base)
     *       event_base_loopcontinue(base);
     *       event_base_loopexit(base, 0);
     */

    evutil_make_socket_nonblocking(fd);
    memset(recv_buf, '\0', max_length);
    ssize_t result = recv(fd, recv_buf, max_length, 0);
    printf("do_read() Event Received (%d - %d) : %s \n", (int)result, do_read_count, recv_buf);

    event_base_loopbreak(base);

}

int main(int argc, char *argv[])
{

    struct sockaddr_in sin;
    struct event *read_event = NULL;

    if ( argc != 3 ) {
        printf("Usgae : %s <url> <port>\n", argv[0]);
        printf("Example:\n");
        printf("  %s 10.0.0.191 1234\n", argv[0]);
        return -1;
    }

    /*
     *  1) get hostname and port number
     *  2) Resolve IP address by hostname  function : gethostbyname();
     *  3) Allocate a new socket,         function : socket()
     *  4) Connect to the remote host.    function : connect()
     *  4) Connect to the remote host.    function : connect()
     *  5) Create event base              function : event_base_new()
     *  6) loop :
     *         get messge from console    function : std:cin.getline()
     *         Send message to socket FD. function : send()
     *         setup read_event           function : event_new()
     *         add read_event to loop     function : event_add()
     *         dispatch event loop        function : event_base_dispatch()
     *  7) close socket FD                function : close(fd)
     *  8) adjust do_read call back
     */

    const char *hostname = argv[1];
    int port_num = std::stoi(argv[2]);
    printf("%s %s %d\n", argv[0], hostname, port_num);

    struct hostent *h = gethostbyname(hostname);
    if ( !h ) {
        printf("gethostbyname() could not resolve hostname\n");
        return -1;
    }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( fd < 0 ) {
        printf("Allocate new socket error\n");
        return -1;
    }
    printf("Allocate new socket FD\n");

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port_num);
    sin.sin_addr = *(struct in_addr*)h->h_addr;

    int rc = connect(fd, (struct sockaddr*) &sin, sizeof(sin));
    if ( rc != 0 ) {
        printf("connect to remote host failed \n");
        close(fd);
        return -1;
    }

    struct event_base *base = event_base_new();      // <-- Creating an event base
    if ( ! base ) {
        printf("Create new event base error\n");
        close(fd);
        return -1;
    }
    printf("Allocated new socket FD and created new event base\n");

    for (;;) {
        evutil_make_socket_nonblocking(fd);
        if ( read_event )
            event_free( read_event);

        std::cout << std::endl << "Input Message : ";
        memset(msg_buf, '\0', max_length);
        std::cin.getline(msg_buf, max_length);

        ssize_t msg_length = strlen(msg_buf);
        if ( msg_length == 0 )
            continue;

        ssize_t n_written = send(fd, msg_buf, msg_length, 0);
        do_read_count = n_written;
        printf("Send Message (%d - %d)\n", (int)msg_length, (int)n_written);

        read_event = event_new(base, fd, EV_READ|EV_PERSIST, do_read, base);
        event_add(read_event, NULL);
        event_base_dispatch(base);
    }

    close(fd);
    event_base_free(base);
    return 0;
}
