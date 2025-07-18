/* standard library */
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>             /* <-- For std::stoi()  */

/* socket includes */
#include <unistd.h>
#include <netinet/in.h>         /* <--  For sockaddr_in  */
#include <sys/socket.h>         /* <--  For socket functions */
#include <netdb.h>              /* <--  For gethostbyname */

/* OpenSSL header files */
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
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }

    /*
     *  2) : create socket an endpoint for communication
     */

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( sockfd < 0 ) {
        printf("ERROR: socket(%d) - %s\n", errno, strerror(errno));
        return EXIT_FAILURE;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port_num);
    sin.sin_addr = *(struct in_addr*)h->h_addr;

    int rc = connect(sockfd, (struct sockaddr*) &sin, sizeof(sin));
    if ( rc != 0 ) {
        printf("ERROR: connect(%d) - %s\n", errno, strerror(errno));
        close(sockfd);
        return EXIT_FAILURE;
    }

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if ( ctx == NULL ) {
        printf("ERROR: could not initialize the SSL context\n");
        return EXIT_FAILURE;
    }
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);

    if ( SSL_connect(ssl) < 0 ) {
        printf("ERROR: could not complete TLS handshake via SSL\n");
        return EXIT_FAILURE;
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
        int sent_bytes = SSL_write(ssl, send_buf, msg_length);
        // ssize_t n_recvd = recv(fd, recv_buf, BUFFER_SIZE, 0);
        int rcvd_bytes = SSL_read(ssl, recv_buf, sent_bytes);
        printf("Recvd Message (%d - %d) : %s \n", rcvd_bytes, sent_bytes, recv_buf);


    }

    SSL_set_shutdown(ssl, SSL_RECEIVED_SHUTDOWN | SSL_SENT_SHUTDOWN);
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    return EXIT_SUCCESS;
}
