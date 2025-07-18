/* standard library */
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>             /* <-- For std::stoi()  */
#include <sstream>              /* <-- For std::stringstream */
#include <thread>               /* <-- For std::thread  */

/* socket includes */
#include <unistd.h>
#include <netinet/in.h>         /* <--  For sockaddr_in  */
#include <sys/socket.h>         /* <--  For socket functions */
#include <netdb.h>              /* <--  For gethostbyname */
#include <fcntl.h>              /* <--  For fcntl - manipulate file descriptor */

/* OpenSSL header files */
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfio.h>

#define BUFFER_SIZE     1024

static void wait_socket_activity(SOCKET_T socket, int flag)
{
    /* Using select system call monitor socket BIO
     * wait until the socket is writeable or readable
     * flag : 0 - read socket
     *        1 = write socket
     */

    fd_set    fds;             /* A struct type of fd sets */
    int nfds;                  /* the highest numberes fd, plus 1 */
    nfds = socket + 1;

    FD_ZERO(&fds);            /* macro clear (removes all fd from) set */
    FD_SET(socket, &fds);     /* macro adds the fd to set */

    if ( flag )
        select(nfds, NULL, &fds, NULL, NULL);
    else
        select(nfds, &fds, NULL, NULL, NULL);
}
void thread_ssl_send(WOLFSSL *ssl, int id )
{
    /*
     * TLS non-blocking Thread function, loop sending 5 messages to server
     */

    printf("thread_ssl_send(%d)\n", id);
    size_t sent_bytes = 0;
    char send_buf[BUFFER_SIZE];
    char buff[256];
    int err;
    memset(buff, '\0', sizeof(buff));

    int sockfd = wolfSSL_get_fd(ssl);

    for ( int i = 0; i < 5; i++ ) { /* Loop Send 5 messages */
        std::stringstream     ss;
        sent_bytes = 0;
        memset(send_buf, '\0', BUFFER_SIZE);
        ss << "Hui Test non-blocking client send from thread(" << id << ") Message(" << i << ")\n";
        memcpy(send_buf, ss.str().c_str(), ss.str().length());

        do {
            sent_bytes = wolfSSL_write(ssl, send_buf, ss.str().length());
            if ( sent_bytes == ss.str().length() ) {
                /* Success */
                printf("SUCCESS: thread_ssl_send(%d loop[%d]) wolfSSL_write[%d - %d] : %s\n",
                        id, i, (int)ss.str().length(), (int)sent_bytes, send_buf);
                break;
            }
            err = wolfSSL_get_error(ssl, sent_bytes);
            if ( err == WOLFSSL_ERROR_WANT_WRITE || err == WOLFSSL_ERROR_WANT_READ ) {
                 printf("RETRYING: thread_ssl_write(%d loop[%d]) wolfSSL_write(%d) - %s\n",
                        id, i, err, wolfSSL_ERR_error_string(err, buff));
                wait_socket_activity(sockfd, 1);   /* retry  write */
                continue;
            } else if ( err == WOLFSSL_ERROR_NONE ) {
                printf("RETRYING: wolfSSL_write(%d) - %s\n", err, "wolfSSL_write() Not Done");
                wait_socket_activity(sockfd, 0);
                continue;
            } else {
                printf("ERROR: thread_ssl_write(%d loop[%d]) wolfSSL_write(%d) - %s\n",
                        id, i, err, wolfSSL_ERR_error_string(err, buff));
                break;
            }
        } while ( err == WOLFSSL_ERROR_WANT_READ || err == WOLFSSL_ERROR_WANT_WRITE );
    }

}

void thread_ssl_read(WOLFSSL *ssl, int id )
{
    /*
     * TLS non-blocking Thread function, loop read 5 echo messages from server
     */

    printf("thread_ssl_read(%d)\n", id);

    size_t rcvd_bytes = 0;
    char rcvd_buf[BUFFER_SIZE];
    int err;
    char buff[256];
    memset(buff, '\0', sizeof(buff));

    int sockfd = wolfSSL_get_fd(ssl);

    for ( int i = 0; i < 5; i++ ) { /* Loop Read 5 Messages */
        rcvd_bytes = 0;
        memset(rcvd_buf, '\0', BUFFER_SIZE);
        do {
            rcvd_bytes = wolfSSL_read(ssl, rcvd_buf, BUFFER_SIZE);
            if ( (int)rcvd_bytes > 0 ) {
                /* Success */
                printf("SUCCESS: thread_ssl_read(%d loop[%d]) Rcvd (%d) : %s \n",
                        id, i, (int)rcvd_bytes, rcvd_buf);
                break;
            }
            err = wolfSSL_get_error(ssl, rcvd_bytes);
            if ( err == WOLFSSL_ERROR_WANT_READ || err == WOLFSSL_ERROR_WANT_WRITE ) {
                printf("RETRYING: thread_ssl_read(%d loop[%d]) wolfSSL_read(%d) - %s\n",
                        id, i, err, wolfSSL_ERR_error_string(err, buff));
                wait_socket_activity(sockfd, 0);   /* retry read*/
                continue;
            } else if ( err == WOLFSSL_ERROR_NONE ) {
                printf("RETRYING: wolfSSL_read(%d) - %s\n", err, "wolfSSL_read() Not Done");
                wait_socket_activity(sockfd, 0);
                continue;
            } else if (err == WOLFSSL_ERROR_ZERO_RETURN ) {
                printf("INFO: thread_ssl_read(%d loop[%d]) wolfSSL_read(%d) - %s\n",
                        id, i, err, wolfSSL_ERR_error_string(err, buff));
                break;
            } else {
                printf("ERROR: thread_ssl_read(%d loop[%d]) wolfSSL_read(%d) - %s\n",
                        id, i, err, wolfSSL_ERR_error_string(err, buff));
                break;
            }
        } while ( err == WOLFSSL_ERROR_WANT_READ || err == WOLFSSL_ERROR_WANT_WRITE);
    }

}

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

    /* Set the socket options to use nonblocking I/O */
    int ret, err;
    ret = fcntl(sockfd, F_SETFL, O_NONBLOCK);
    if ( ret == -1 ) {
        printf("ERROR: fcntl(%d) - %s\n", errno, strerror(errno));
        return EXIT_FAILURE;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port_num);
    sin.sin_addr = *(struct in_addr*)h->h_addr;

    while ( connect(sockfd, (struct sockaddr*) &sin, sizeof(sin)) == -1 ) {
        if ( errno == EAGAIN || errno == EWOULDBLOCK ) {
            /* non-blockinf connect, wait for read activity on socket */
            wait_socket_activity(sockfd, 0);
            continue;
        } else if ( errno == EINPROGRESS || errno == EALREADY ) {
            break;
        }
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
    char buff[256];
    memset(buff, '\0', sizeof(buff));

    ret = wolfSSL_set_fd(ssl, sockfd);
    if ( ret != WOLFSSL_SUCCESS ) {
        err = wolfSSL_get_error(ssl, ret);
        printf("ERROR: wolfSSL_set_fd(%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
        return EXIT_FAILURE;
    }

    /* Remove doing client authentication on the server */
    wolfSSL_set_verify(ssl, SSL_VERIFY_NONE, NULL);

    /* Connect and Initialize handshake  */
    do {
        ret = wolfSSL_connect(ssl);
        if ( ret == WOLFSSL_SUCCESS ) {
            printf("INFO: wolfSSL_connect() Success\n");
            break;
        }
        err = wolfSSL_get_error(ssl, ret);
        if ( err == WOLFSSL_ERROR_WANT_CONNECT ||
             err == WOLFSSL_ERROR_WANT_READ ||
             err == WOLFSSL_ERROR_WANT_WRITE ) {
            /* non-blocking connect, wait for socket ready activity */
            printf("RETRYING: wolfSSL_connect(%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
            wait_socket_activity(sockfd, 0);
            continue;
        } else if ( err == WOLFSSL_ERROR_NONE ) {
            printf("RETRYING: wolfSSL_connect(%d) - %s\n", err, "wolfSSL_connect() Not Done");
            wait_socket_activity(sockfd, 0);
            continue;
        } else {
            printf("ERROR: wolfSSL_connect(%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
            break;
        }
    } while ( ret != WOLFSSL_SUCCESS &&
            ( err == WOLFSSL_ERROR_WANT_READ || err == WOLFSSL_ERROR_WANT_WRITE ||
              err == WOLFSSL_ERROR_NONE));

    for ( ;; ) {
        std::cout << std::endl << "Input Message : ";
        memset(recv_buf, '\0', BUFFER_SIZE);
        memset(send_buf, '\0', BUFFER_SIZE);
        std::cin.getline(send_buf, BUFFER_SIZE);
        int sent_bytes = 0;
        int rcvd_bytes = 0;

        ssize_t msg_length = strlen(send_buf);
        if ( msg_length == 0 ) {
            break;
        }

        do {
            sent_bytes = wolfSSL_write(ssl, send_buf, msg_length);
            if ( sent_bytes == msg_length ) {
                printf("SUCCESS: wolfSSL_write(%d - %d) | %s\n", (int)msg_length, sent_bytes, send_buf);
                break;
            }
            err = wolfSSL_get_error(ssl, ret);
            if ( err == WOLFSSL_ERROR_WANT_READ ||
                 err == WOLFSSL_ERROR_WANT_WRITE ) {
                printf("RETRYING: wolfSSL_write(%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
                wait_socket_activity(sockfd, 1);
                continue;
            } else if ( err == WOLFSSL_ERROR_NONE ) {
                printf("RETRYING: wolfSSL_write(%d) - %s\n", err, "wolfSSL_write() Not Done");
                wait_socket_activity(sockfd, 0);
                continue;
            } else {
                printf("ERROR: wolfSSL_write(%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
            }
        } while (err == WOLFSSL_ERROR_WANT_READ ||
                 err == WOLFSSL_ERROR_WANT_WRITE ||
                 err == WOLFSSL_ERROR_NONE);

        do {
            rcvd_bytes = wolfSSL_read(ssl, recv_buf, sent_bytes);
            if ( rcvd_bytes == sent_bytes ) {
                /* Success */
                printf("SUCCESS: wolfSSL_read(%d - %d) : %s \n", rcvd_bytes, sent_bytes, recv_buf);
                break;
            }
            err = wolfSSL_get_error(ssl, ret);
            if ( err == WOLFSSL_ERROR_WANT_READ ||
                 err == WOLFSSL_ERROR_WANT_WRITE ) {
                printf("RETRYING: wolfSSL_read(%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
                wait_socket_activity(sockfd, 0);
                continue;
            } else if ( err == WOLFSSL_ERROR_ZERO_RETURN ) {
                printf("INFO: wolfSSL_read(%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
                break;
            } else if ( err == WOLFSSL_ERROR_NONE ) {
                printf("RETRYING: wolfSSL_read(%d) - %s\n", err, "wolfSSL_read() Not Done");
                wait_socket_activity(sockfd, 0);
                continue;
            } else {
                printf("ERROR: wolfSSL_read(%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
                break;
            }
        } while (err == WOLFSSL_ERROR_WANT_READ ||
                 err == WOLFSSL_ERROR_WANT_WRITE ||
                 err == WOLFSSL_ERROR_NONE);

    }

    std::thread   first (thread_ssl_send, ssl, 1);
    std::thread   second(thread_ssl_read, ssl, 4);
    first.join();
    second.join();

    do {
        ret = wolfSSL_shutdown(ssl);        /* shutdown an active TLS/SSL connection */
        if ( ret == WOLFSSL_SUCCESS ) {
            printf("INFO: wolfSSL_shutdown() Success\n");
            break;
        }
        err = wolfSSL_get_error(ssl, ret);
        if ( err == WOLFSSL_ERROR_WANT_READ ||
             err == WOLFSSL_ERROR_WANT_WRITE ) {
            printf("RETRYING: wolfSSL_shutdow(%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
            wait_socket_activity(sockfd, 0);
            continue;
        } else if ( err == WOLFSSL_ERROR_NONE ) {
            printf("RETRYING: wolfSSL_shutdow(%d) - %s\n", err, "wolfSSL Shutdown Not Done");
            wait_socket_activity(sockfd, 0);
            continue;
        } else {
            printf("ERROR: (%d) - %s\n", err, wolfSSL_ERR_error_string(err, buff));
            break;
        }
    } while (err == WOLFSSL_ERROR_WANT_READ || err == WOLFSSL_ERROR_WANT_WRITE ||
             err == WOLFSSL_ERROR_NONE);

    close(sockfd);                /* close the socket */
    wolfSSL_CTX_free(ctx);        /* Free the wolfSSL context object */
    wolfSSL_Cleanup();            /* Cleanup the wolfSSL environment */
    return EXIT_SUCCESS;
}
