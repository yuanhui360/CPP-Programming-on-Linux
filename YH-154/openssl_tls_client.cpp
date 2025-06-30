#include <iostream>       // std::cout
// #include <thread>         // std::thread
// #include <vector>         // std::vector
// #include <sstream>        // std::stringstream

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

    if ((rc = SSL_connect(ssl)) <= 0) {
        printf("Failed to connect to server\n");
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

        int send_bytes = SSL_write(ssl, send_buf, msg_length);
        int rcvd_bytes = SSL_read(ssl, recv_buf, send_bytes);
        printf("Recvd Message (%d - %d) : %s \n", rcvd_bytes, send_bytes, recv_buf);

    }

    if ((rc = SSL_shutdown(ssl)) < 0) {
        printf("Error shutting down %d\n", rc);
    }

    SSL_free(ssl);
    SSL_CTX_free(ctx);
    return  EXIT_SUCCESS;
}
