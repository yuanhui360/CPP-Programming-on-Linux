#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>
#define BUFFER_SIZE     1024

int main(int argc, char **argv)
{
    char msg_buf[BUFFER_SIZE];
    int rc;

    if ( argc != 4 ) {
        printf("usage: %s <port> <certificate> <private_key>\n", argv[0]);
        printf("Example : %s 1234 my_certificate.pem my_private_key.pem\n", argv[0]);
        return 1;
    }

    int port_num = std::stoi(argv[1]);
    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
    if (ctx == nullptr ) {
        printf("Unable to create SSL context\n");
        exit(1);
    }

    rc = SSL_CTX_use_certificate_file(ctx, argv[2], SSL_FILETYPE_PEM);
    if ( rc <= 0 ) {
        printf("Set SSL_CTX_use_certificate_file() error\n");
        exit(1);
    }

    rc = SSL_CTX_use_PrivateKey_file(ctx, argv[3], SSL_FILETYPE_PEM);
    if ( rc <= 0 ) {
        printf("Set SSL_CTX_use_PrivateKey_file() error\n");
        exit(1);
    }

    struct sockaddr_in    sin;
    sin.sin_family        = AF_INET;       // <-- IPv4 internet protcol
    sin.sin_addr.s_addr   = INADDR_ANY;    // <-- Accept any incoming messages (0)
    sin.sin_port          = htons(port_num);

    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    rc = bind(listener, (struct sockaddr*) &sin, sizeof(sin));
    if ( rc < 0 ) {
        printf("bind socket error, port number : <%d>, listener : %d\n", port_num, listener);
        exit(1);
    }

    rc = listen(listener, 16);
    if ( rc < 0 ) {
        printf("listen for connection error, listener : <%d>\n", listener);
        exit(1);
    }

    printf("SSL/TLS Echo Server started, port number : (%d)\n", port_num);
    while(1) {
        struct sockaddr_in addr;
        unsigned int len = sizeof(addr);

        int fd = accept(listener, (struct sockaddr*)&addr, &len);
        if (fd < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, fd);
        if ( SSL_accept(ssl) <= 0 ) {
            printf("Unable to accept SSL handshake\n");
        }

        for ( ;; ) {
            memset(msg_buf, '\0', BUFFER_SIZE);
            // ssize_t n_recvd = recv(fd, msg_buf, BUFFER_SIZE, 0);
            int n_recvd = SSL_read(ssl, msg_buf, BUFFER_SIZE);
            if ( n_recvd <= 0 )
                break;
            // ssize_t n_send = send(fd, msg_buf, n_recvd, 0);
            int n_send = SSL_write(ssl, msg_buf, n_recvd);
            if ( n_send <= 0 )
                break;
            printf("Recvd Message  (%d - %d) : %s \n", (int)n_recvd, (int)n_send, msg_buf);
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(fd);
    }

    close(listener);
    SSL_CTX_free(ctx);
    return 0;
}

