lude <netinet/in.h>         /* <--  For sockaddr_in  */
#include <sys/socket.h>         /* <--  For socket functions */
#include <netdb.h>              /* <--  For gethostbyname */

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <sstream>

#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUFFER_SIZE     1024

int main(int argc, char *argv[])
{

    struct sockaddr_in sin;
    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];

    if ( argc != 3 ) {
        printf("Usgae : %s <host|url> <port>\n", argv[0]);
        printf("Example:\n");
        printf("  %s 10.0.0.191 1234\n", argv[0]);
        return -1;
    }

    /*
     *  1) : Get Hostname, port number from command line
     *       resolve host by hostname
     */

    const char *hostname = argv[1];
    int port_num = std::stoi(argv[2]);
    printf("%s %s %d\n", argv[0], hostname, port_num);

    struct hostent *h = gethostbyname(hostname);
    if ( !h ) {
        printf("gethostbyname() could not resolve hostname\n");
        return -1;
    }

    /*
     *  2) : create socket an endpoint for communication
     */

    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( fd < 0 ) {
        printf("create new socket error\n");
        return -1;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port_num);
    sin.sin_addr = *(struct in_addr*)h->h_addr;

    int rc = connect(fd, (struct sockaddr*) &sin, sizeof(sin));
    if ( rc != 0 ) {
        printf("connect to remote host failed \n");
        close(fd);
        return -1;
    }

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if ( ctx == NULL ) {
        printf("ERROR: could not initialize the SSL context\n");
        exit(1);
    }
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, fd);

    if ( SSL_connect(ssl) < 0 ) {
        printf("ERROR: could not complete TLS handshake via SSL\n");
        exit(1);
    }

    for ( ;; ) {
        std::cout << std::endl << "Input Message : ";
        memset(recv_buf, '\0', BUFFER_SIZE);
        memset(send_buf, '\0', BUFFER_SIZE);
        std::cin.getline(send_buf, BUFFER_SIZE);

        ssize_t msg_length = strlen(send_buf);
        if ( msg_length == 0 ) {
            break;
        }

        // ssize_t n_send = send(fd, send_buf, msg_length, 0);
        int n_send = SSL_write(ssl, send_buf, msg_length);
        // ssize_t n_recvd = recv(fd, recv_buf, BUFFER_SIZE, 0);
        int n_recvd = SSL_read(ssl, recv_buf, n_send);
        printf("Recvd Message (%d - %d) : %s \n", (int)n_recvd, (int)n_send, recv_buf);


    }


    SSL_set_shutdown(ssl, SSL_RECEIVED_SHUTDOWN | SSL_SENT_SHUTDOWN);
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(fd);
    return 0;
}
