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
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>

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

    wolfSSL_Init();             /* <-- Initialize wolfSSL  */

    WOLFSSL_CTX    *ctx = NULL;
    WOLFSSL        *ssl = NULL;

    /* Create and Initialize WOLFSSL_CTX */
    ctx = wolfSSL_CTX_new(wolfSSLv23_client_method());
    if ( ctx == NULL ) {
        printf("ERROR: wolfSSL_CTX_new() could not initialize the WOLFSSL context\n");
        return EXIT_FAILURE;
    }

    /* Create and Initialize WOLFSSL Object  */

    ssl = wolfSSL_new(ctx);
    if ( ssl == NULL ) {
        printf("ERROR: wolfSSL_new() could not initialize the WOLFSSL Object\n");
        return EXIT_FAILURE;
    }

    /* Attach wolfSSL to the socket */
    int ret, err;
    char buff[256];
    memset(buff, '\0', sizeof(buff));

    ret = wolfSSL_set_fd(ssl, sockfd);
    if ( ret != WOLFSSL_SUCCESS ) {
        err = wolfSSL_get_error(ssl, ret);
        printf("ERROR: (%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
        return EXIT_FAILURE;
    }

    /* Remove doing client authentication on the server */
    wolfSSL_set_verify(ssl, SSL_VERIFY_NONE, NULL);

    /* Connect and Initialize handshake  */
    ret = wolfSSL_connect(ssl);
    if ( ret != WOLFSSL_SUCCESS ) {
        err = wolfSSL_get_error(ssl, ret);
        printf("ERROR: (%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
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

        int sent_bytes = wolfSSL_write(ssl, send_buf, msg_length);
        if ( sent_bytes <= 0 ) {
            err = wolfSSL_get_error(ssl, ret);
            printf("ERROR: (%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
        }
        int rcvd_bytes = wolfSSL_read(ssl, recv_buf, sent_bytes);
        if ( rcvd_bytes > 0 ) {
            printf("Recvd Message (%d - %d) : %s \n", rcvd_bytes, sent_bytes, recv_buf);
        } else {
            err = wolfSSL_get_error(ssl, ret);
            printf("ERROR: (%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
        }


    }

    wolfSSL_shutdown(ssl);        /* shutdown an active TLS/SSL connection */
    close(sockfd);                /* close the socket */
    wolfSSL_CTX_free(ctx);        /* Free the wolfSSL context object */
    wolfSSL_Cleanup();            /* Cleanup the wolfSSL environment */
    return EXIT_SUCCESS;
}
