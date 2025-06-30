#include <iostream>       // std::cout
#include <thread>         // std::thread
// #include <vector>         // std::vector
#include <sstream>        // std::stringstream

#include <string.h>
#include <string>

# include <sys/socket.h>
# include <sys/select.h>
#include <sys/types.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUFFER_SIZE     1024

static BIO *create_socket_bio(const char *hostname, const char *port)
{
    int sock = -1;
    BIO_ADDRINFO *res;
    BIO *bio;

    if (!BIO_lookup_ex(hostname, port, BIO_LOOKUP_CLIENT, AF_INET, SOCK_STREAM, 0, &res))
        return NULL;

    sock = BIO_socket(BIO_ADDRINFO_family(res), SOCK_STREAM, 0, 0);
    if (sock == -1)
        return NULL;

    /* Connect the socket to the server's address */
    if (!BIO_connect(sock, BIO_ADDRINFO_address(res), BIO_SOCK_NODELAY)) {
        BIO_closesocket(sock);
        sock = -1;
    }

    /* Set to nonblocking mode */
    if ( !BIO_socket_nbio(sock, 1)) {
        sock = -1;
    }

    /* Free the address information resources we allocated earlier */
    BIO_ADDRINFO_free(res);

    /* If sock is -1 then we've been unable to connect to the server */
    if (sock == -1)
        return NULL;

    /* Create a BIO to wrap the socket */
    bio = BIO_new(BIO_s_socket());
    if (bio == NULL) {
        BIO_closesocket(sock);
        return NULL;
    }

    BIO_set_fd(bio, sock, BIO_CLOSE);

    return bio;
}

static void wait_for_activity(SSL *ssl, int flag)
{
    /* Using select system call monitor socket BIO
     * wait until the socket is writeable or readable
     * flag : 0 - read socket
     *        1 = write socket
     */

    fd_set    fds;   /* A struct type of fd sets */
    int nfds;        /* the highest numberes fd, plus 1 */
    int sock;        /* socket fd */

    sock = SSL_get_fd(ssl);   /* Get socket fd */
    nfds = sock + 1;
    FD_ZERO(&fds);            /* macro clear (removes all fd from) set */
    FD_SET(sock, &fds);       /* macro adds the fd to set */

    if ( flag )
        select(nfds, NULL, &fds, NULL, NULL);
    else
        select(nfds, &fds, NULL, NULL, NULL);
}

static int handle_io_failure(SSL *ssl, int rc)
{
    /*
     *  Check Error and decide whether to :
     *      1 <-- Retry
     *      0 <-- EOF, forward
     *     -1 <-- Failure
     */

    int ret;
    switch (SSL_get_error(ssl, rc)) {
        case SSL_ERROR_WANT_READ:
            wait_for_activity(ssl, 0);   /* wait for read activity ready */
            ret = 1;
            break;
        case SSL_ERROR_WANT_WRITE:
            wait_for_activity(ssl, 1);  /* wait for write activity ready */
            ret = 1;
            break;
        case SSL_ERROR_ZERO_RETURN:
            ret = 0;
            break;
        case SSL_ERROR_SYSCALL:
            ret = -1;
            break;
        case SSL_ERROR_SSL:
            ret = -1;
            break;
        default:
            ret = -1;
            break;
    }
    return ret;
}

void thread_ssl_send(SSL *ssl, int id )
{
    printf("thread_ssl_send(%d)\n", id);
    size_t sent_bytes = 0;
    char send_buf[BUFFER_SIZE];

    for ( int i = 0; i < 5; i++ ) { /* Loop Send 5 messages */
        std::stringstream     ss;
        sent_bytes = 0;
        memset(send_buf, '\0', BUFFER_SIZE);
        ss << "Hui Test non-blocking client send from thread(" << id << ") Message(" << i << ")\n";
        memcpy(send_buf, ss.str().c_str(), ss.str().length());
        printf("thread_ssl_send(%d) Bytes(%d) : %s\n", id, (int)ss.str().length(), send_buf);
        for ( int j = 0; j < 5; j++ ) { /* for each message loop retry 5 times if error */
            if (SSL_write_ex(ssl, send_buf, strlen(send_buf), &sent_bytes) == 1 ) {
                /* Success */
                break;
            }
            if (handle_io_failure(ssl, 0) == 1) {
                printf("Retrying thread_ssl_send(%d) : Message(%d) - Loop(%d)\n", id, i, j);
                continue;  /* retry */
            }
            printf("Failed thread_ssl_send(%d) : Message(%d) - Loop(%d)\n", id, i, j);
        }
    }
}

void thread_ssl_read(SSL *ssl, int id )
{
    printf("thread_ssl_read(%d)\n", id);

    size_t read_bytes = 0;
    char rcvd_buf[BUFFER_SIZE];

    for ( int i = 0; i < 5; i++ ) { /* Loop Read 5 Messages */
        read_bytes = 0;
        memset(rcvd_buf, '\0', BUFFER_SIZE);
        for ( int j = 0; j < 5; j++ ) {  /* for each message loop retry 5 times if error */
            int rc = 0;
            if ( SSL_read_ex(ssl, rcvd_buf, BUFFER_SIZE, &read_bytes) == 1 ) {
                /* Success */
                break;
            }
            switch ( handle_io_failure(ssl, 0)) {
                case 1:
                    rc = 1;
                    printf("Retrying thread_ssl_read(%d) : Message(%d) - Loop(%d)\n", id, i, j);
                    break;
                case 0:
                    rc = 0;
                    printf("EOF thread_ssl_read(%d) : Message(%d) - Loop(%d)\n", id, i, j);
                    break;
                case -1:
                default:
                    rc = -1;
                    printf("Error thread_ssl_read(%d) : Message(%d) - Loop(%d)\n", id, i, j);
                    break;
            }
            if ( rc == 1 ) {
                continue;
            } else {
                break;
            }
        }
        printf("Thread(%d) Received %d Bytes : %s\n", id, (int)read_bytes, rcvd_buf);
    }
}

int main(int argc, char *argv[])
{
    BIO *bio = NULL;
    SSL *ssl = NULL;
    int rc = EXIT_FAILURE;

    if ( argc != 3 ) {
        printf("Usgae : %s <host|url> <port>\n", argv[0]);
        printf("Example:\n");
        printf("  %s 10.0.0.191 1234\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *hostname = argv[1];
    const char *port = argv[2];
    int port_num = std::stoi(argv[2]);
    printf("%s %s %d\n", argv[0], hostname, port_num);

    /*
     * Create an SSL_CTX which we can use to create SSL objects from. We
     * want an SSL_CTX for creating clients so we use TLS_client_method()
     * here.
     */

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (ctx == NULL) {
        printf("ERROR: could not initialize the SSL context\n");
        exit(-1);
    }

    /*
     * Create an SSL object to represent the TLS connection
     */

    ssl = SSL_new(ctx);
    if (ssl == NULL) {
        printf("Failed to create the SSL object\n");
        return EXIT_FAILURE;
    }

    bio = create_socket_bio(hostname, port);
    if (bio == NULL) {
        printf("Failed to crete the BIO\n");
        return EXIT_FAILURE;
    }
    SSL_set_bio(ssl, bio, bio);

    while ((rc = SSL_connect(ssl)) != 1) {
        if ( handle_io_failure(ssl, rc) == 1 ) {
            continue;
        }
        printf("Failed to connect (handsshake) to server\n");
        return EXIT_FAILURE;
    }

    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];

    for ( ;; ) {
        std::cout << std::endl << "Input Message : ";
        memset(recv_buf, '\0', BUFFER_SIZE);
        memset(send_buf, '\0', BUFFER_SIZE);
        std::cin.getline(send_buf, BUFFER_SIZE);

        ssize_t msg_length = strlen(send_buf);
        if ( msg_length == 0 ) {
            break;
        }

        // int send_bytes = SSL_write(ssl, send_buf, msg_length);
        // int rcvd_bytes = SSL_read(ssl, recv_buf, send_bytes);
        size_t send_bytes = 0;
        size_t rcvd_bytes = 0;

        while ( !SSL_write_ex(ssl, send_buf, msg_length, &send_bytes)) {
            if ( handle_io_failure(ssl, 0) == 1)
                continue;
            printf("Failed to write message to socket\n");
        }

        while (!SSL_read_ex(ssl, recv_buf, send_bytes, &rcvd_bytes)) {
            switch (handle_io_failure(ssl, 0)) {
                case 1:
                    rc = 1;
                    printf("Retrying reading remaining message\n");
                    break;    /* retry */
                case 0:
                    rc = 0;
                    printf("EOF during reading remaining message\n");
                    break;
                case -1:
                default:
                    rc = -1;
                    printf("Failed during reading remaining message\n");
                    break;
            }
            if ( rc == 1 )
                continue;
            else
                break;
        }

        printf("Recvd Message (%d - %d) : %s \n", (int)rcvd_bytes, (int)send_bytes, recv_buf);

    }

    std::thread first (thread_ssl_send, ssl, 1);
    std::thread second(thread_ssl_read, ssl, 4);
    first.join();
    second.join();

    while ((rc = SSL_shutdown(ssl)) != 1) {
        if ( rc < 0 && handle_io_failure(ssl, rc) == 1) {
            continue;   /* retry */
        } else if ( rc == 0 ) {
            break;
        }
        printf("Error shutting down %d\n", rc);
        break;
    }

    SSL_free(ssl);
    SSL_CTX_free(ctx);
    return  EXIT_SUCCESS;
}
