#include <stdio.h>
#include <unistd.h>
#include <string.h>
// #include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/ssl.h>
#include <wolfssl/error-ssl.h>

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

    rc = wolfSSL_Init();
    if (rc != SSL_SUCCESS) {
        printf("failed to initialize wolfSSL library \n");
        exit(-1);
    }
    wolfSSL_Debugging_ON();                    // Optionally set debugging on
    printf("wolfSSL library version : %s\n", wolfSSL_lib_version());

    int port_num = std::stoi(argv[1]);

    // Replace OpenSSL SSL_CTX object to wolfSSL_CTX object
    WOLFSSL_CTX * server_ctx = NULL;
    server_ctx = wolfSSL_CTX_new(wolfSSLv23_server_method());
    if ( server_ctx == NULL ) {
        printf("Error: Unable to create WOLFSSL context\n");
        exit(1);
    }

    rc =  wolfSSL_CTX_use_certificate_file(server_ctx, argv[2], WOLFSSL_FILETYPE_PEM);
    if ( rc != WOLFSSL_SUCCESS) {
        printf("Set wolfSSL_CTX_use_certificate_file() error\n");
        exit(1);
    }

    rc =  wolfSSL_CTX_use_PrivateKey_file(server_ctx, argv[3], WOLFSSL_FILETYPE_PEM);
    if ( rc != WOLFSSL_SUCCESS) {
        printf("Set wolfSSL_CTX_use_PrivateKey_file() error\n");
        exit(1);
    }

    struct sockaddr_in    sin;
    sin.sin_family        = AF_INET;       // <-- IPv4 internet protcol
    sin.sin_addr.s_addr   = INADDR_ANY;    // <-- Accept any incoming messages (0)
    sin.sin_port          = htons(port_num);

    SOCKET_T listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

    printf("wolfSSL/TLS Echo Server started, port number : (%d)\n", port_num);
    while(1) {
        struct sockaddr_in addr;
        unsigned int len = sizeof(addr);

        SOCKET_T fd = accept(listener, (struct sockaddr*)&addr, &len);
        if (fd < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        WOLFSSL *ssl = wolfSSL_new(server_ctx);
        if (ssl == NULL) {
           // SSL object creation failed
            printf("Unable to start SSL session\n");
        }

        wolfSSL_set_fd(ssl, fd);
        rc = wolfSSL_accept(ssl);
        if ( rc != WOLFSSL_SUCCESS ) {
            printf("Unable to accept SSL handshake\n");
        }

        for ( ;; ) {
            memset(msg_buf, '\0', BUFFER_SIZE);
            // ssize_t n_recvd = recv(fd, msg_buf, BUFFER_SIZE, 0);
            int n_recvd = wolfSSL_read(ssl, msg_buf, BUFFER_SIZE);
            if ( n_recvd <= 0 )
                break;
            // ssize_t n_send = send(fd, msg_buf, n_recvd, 0);
            int n_send = wolfSSL_write(ssl, msg_buf, n_recvd);
            if ( n_send <= 0 )
                break;
            printf("Recvd Message  (%d - %d) : %s \n", (int)n_recvd, (int)n_send, msg_buf);
        }

        wolfSSL_shutdown(ssl);
        wolfSSL_free(ssl);
        CloseSocket(fd);
    }

    close(listener);
    wolfSSL_CTX_free(server_ctx);
    wolfSSL_Cleanup();
    return 0;
}
