#include <netinet/in.h>         /* <--  For sockaddr_in  */
#include <sys/socket.h>         /* <--  For socket functions */
#include <netdb.h>              /* <--  For gethostbyname */

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <sstream>

const int max_length = 1024;

int main(int argc, char *argv[])
{
    /*
    struct hostent *h;
    const char *cp;
    int fd;
    ssize_t n_written, remaining;

     */

    struct sockaddr_in sin;
    char recv_buf[max_length];
    char send_buf[max_length];

    if ( argc != 3 ) {
        printf("Usgae : %s <url> <port>\n", argv[0]);
        printf("Example:\n");
        printf("  %s 10.0.0.191 1234\n", argv[0]);
        return -1;
    }

    /*
     *  1) get hostname and port number
     *  2) Resolve IP address by hostname  function : gethostbyname();
     *  3) Allocate a new socket,          function : socket()
     *     IP addr type : AF_INET
     *  4) Connect to the remote host.     function : connect()
     *  5) loop :
     *         get messge from console     function : std:cin.getline()
     *         Send message to socket FD.  function : send()
     *         Read from socket FD.        fundtion : recv()
     *  6) close socket FD                 function : close(fd)
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

    for ( ;; ) {
        std::cout << std::endl << "Input Message : ";
        memset(recv_buf, '\0', max_length);
        memset(send_buf, '\0', max_length);
        std::cin.getline(send_buf, max_length);

        ssize_t msg_length = strlen(send_buf);
        if ( msg_length == 0 ) {
            break;
        }

        ssize_t n_written = send(fd, send_buf, msg_length, 0);
        ssize_t recvd_length = recv(fd, recv_buf, max_length, 0);
        printf("Recvd Message (%d - %d) : %s \n", (int)recvd_length, (int)n_written, recv_buf);

    }
    close(fd);
    return 0;
}
