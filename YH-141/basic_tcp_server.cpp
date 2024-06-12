#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <string>
#define BUFFER_SIZE     1024

int main(int argc, char **argv)
{
    char msg_buf[BUFFER_SIZE];

    if ( argc != 2 ) {
        printf("usage: %s <port>\n", argv[0]);
        printf("Example : %s 1234\n", argv[0]);
        return 1;
    }

    int port_num = std::stoi(argv[1]);

    struct sockaddr_in    sin;
    sin.sin_family        = AF_INET;       // <-- IPv4 internet protcol
    sin.sin_addr.s_addr   = INADDR_ANY;    // <-- Accept any incoming messages (0)
    sin.sin_port          = htons(port_num);

    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int rc = bind(listener, (struct sockaddr*) &sin, sizeof(sin));
    if ( rc < 0 ) {
        printf("bind socket error, port number : <%d>, listener : %d\n", port_num, listener);
        exit(1);
    }

    rc = listen(listener, 16);
    if ( rc < 0 ) {
        printf("listen for connection error, listener : <%d>\n", listener);
        exit(1);
    }

    printf("TCP Echo Server started, port number : (%d)\n", port_num);
    while(1) {
        struct sockaddr_in addr;
        unsigned int len = sizeof(addr);

        int fd = accept(listener, (struct sockaddr*)&addr, &len);
        if (fd < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        for ( ;; ) {
            memset(msg_buf, '\0', BUFFER_SIZE);
            ssize_t n_recvd = recv(fd, msg_buf, BUFFER_SIZE, 0);
            if ( n_recvd <= 0 )
                break;
            ssize_t n_send = send(fd, msg_buf, n_recvd, 0);
            if ( n_send <= 0 )
                break;
            printf("Recvd Message  (%d - %d) : %s \n", (int)n_recvd, (int)n_send, msg_buf);
        }

        close(fd);
    }

    close(listener);
    return 0;
}
