#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <netinet/in.h>         /* <--  For sockaddr_in  */
#include <netdb.h>              /* <--  For gethostbyname */
#include <sys/socket.h>         /* <--  For sockaddr_in  */
#include <arpa/inet.h>

#include <vector>
#include <string>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#define BUFFER_SIZE     1024

int main(int argc, char **argv)
{
    char msg_buf[BUFFER_SIZE];
    char dst_buf[BUFFER_SIZE];

    if ( argc != 3 ) {
        printf("usage: %s <port> <redirect [host:port]>\n", argv[0]);
        printf("Example : %s 1234 10.0.0.191:4567\n", argv[0]);
        return 1;
    }

    int port_num = std::stoi(argv[1]);

    std::string str = std::string(argv[2]);
    std::vector<std::string> dest_vec;
    split(dest_vec, str, boost::algorithm::is_any_of(":"));  // <-- split str by : (host:port)

    struct sockaddr_in    sin;
    sin.sin_family        = AF_INET;       // <-- IPv4 internet protcol
    sin.sin_addr.s_addr   = INADDR_ANY;    // <-- Accept any incoming messages (0)
    sin.sin_port          = htons(port_num);

    struct hostent *h = gethostbyname(dest_vec[0].c_str());
    struct sockaddr_in    dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr = *(struct in_addr*)h->h_addr;
    dest_addr.sin_port = htons(stoi(dest_vec[1]));

    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int rc = bind(listener, (struct sockaddr*) &sin, sizeof(sin));
    if ( rc < 0 ) {
        printf("bind socket error, port number : <%d>, listener : %d\n", port_num, listener);
        exit(EXIT_FAILURE);
    }


    rc = listen(listener, 16);
    if ( rc < 0 ) {
        printf("listen for connection error, listener : <%d>\n", listener);
        exit(EXIT_FAILURE);
    }

    printf("Proxy Server started, port number : (%d)\n", port_num);
    while(1) {
        struct sockaddr_in addr;
        unsigned int len = sizeof(addr);

        int fd = accept(listener, (struct sockaddr*)&addr, &len);
        if (fd < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        int dst_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (dst_fd < 0) {
            perror("Unable to connect dest host");
            exit(EXIT_FAILURE);
        }
        rc = connect(dst_fd, (struct sockaddr*) &dest_addr, sizeof(dest_addr));
        if ( rc != 0 ) {
            printf("connect to dest host failed \n");
            close(dst_fd);
            exit(-1);
        }

        printf("Proxy Server redirect connected to destination : (%s)\n", argv[2]);
        for ( ;; ) {
            memset(msg_buf, '\0', BUFFER_SIZE);
            ssize_t n_recvd = recv(fd, msg_buf, BUFFER_SIZE, 0);
            if ( n_recvd <= 0 )
                break;
            ssize_t n_send = send(dst_fd, msg_buf, n_recvd, 0);
            if ( n_send <= 0 )
                break;

            printf("Redirected Message  (%d - %d) : %s \n", (int)n_recvd, (int)n_send, msg_buf);

            memset(dst_buf, '\0', BUFFER_SIZE);
            n_recvd = recv(dst_fd, dst_buf, BUFFER_SIZE, 0);
            if ( n_recvd <= 0 )
                break;
            n_send = send(fd, dst_buf, n_recvd, 0);
            if ( n_send <= 0 )
                break;

            printf("Responded Message  (%d - %d) : %s \n", (int)n_recvd, (int)n_send, dst_buf);
        }

        close(dst_fd);
        close(fd);
        printf("Redirection Disconnected (%s)\n", argv[2]);
    }

    close(listener);
    return 0;
}
