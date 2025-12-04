#include <iostream>
#include <sys/statvfs.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <libssh/libssh.h>
#include <vector>

#define BUFFER_SIZE     1024
struct TH_PARAM
{
    int       id;
    char      Server[256];
    int       Port;
    char      Login[256];
    char      PassWd[256];
    std::vector<std::string> cmdVec;
    TH_PARAM() : id(0), Port(0)
    {
        memset(Server,'\0', sizeof(Server));
        memset(Login, '\0', sizeof(Login));
        memset(PassWd, '\0', sizeof(PassWd));
    }
};

/*
 * Under Linux, this function determines whether a key has been pressed.
 */

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(0, &fds);

    return select(1, &fds, NULL, NULL, &tv);
}

int ssh_session_function( const struct TH_PARAM& inParam )
{
    int rc = 0;

    // step 1 start a new SSH session
    // rc = ssh_init();
    ssh_session session = ssh_new();
    if( session == NULL) {
        return -1;
    }

    // step 2 setup session options
    int verbosity = SSH_LOG_NOLOG;
    ssh_options_set(session, SSH_OPTIONS_HOST, inParam.Server);
    ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(session, SSH_OPTIONS_PORT, &inParam.Port);
    ssh_options_set(session, SSH_OPTIONS_USER, inParam.Login);

    // step 3 session connect to remote
    rc = ssh_connect(session);
    if (rc == SSH_ERROR)
    {
        printf("ERROR: ssh_connect(%d) - %s", ssh_get_error_code(session), ssh_get_error(session));
        return -1;
    }

    // step 4 authenticate login/password
    rc = ssh_userauth_password(session, inParam.Login, inParam.PassWd);
    if (rc != SSH_AUTH_SUCCESS)
    {
        printf("ERROR: ssh_userauth_password(%d) - %s", ssh_get_error_code(session), ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return SSH_ERROR;
    }

    // Step 5 Allocate a new channel. NULL on error
    ssh_channel channel = ssh_channel_new(session);
    if ( channel == NULL ) {
        printf("ERROR: ssh_channel_new(%d) - %s",
                ssh_get_error_code(session),
                ssh_get_error(session));
        return SSH_ERROR;
    }

    // Step 6 channel open new sub-session
    rc = ssh_channel_open_session(channel);
    if ( rc == SSH_ERROR ) {
        printf("ERROR: ssh_channel_open_session(%d) - %s",
                ssh_get_error_code(session),
                ssh_get_error(session));
        return SSH_ERROR;
    }

    // step 8 Loop Interactive communicate with remote SSH server

    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];
    memset(recv_buf, '\0', BUFFER_SIZE);
    memset(send_buf, '\0', BUFFER_SIZE);
    int nbytes, nwritten;
    (void) nbytes;
    (void) nwritten;

    write(1, "Type Message : ", sizeof("Type Message : "));
    do {
        nbytes = ssh_channel_read_nonblocking(channel, recv_buf, BUFFER_SIZE, 0);
        if ( nbytes > 0 ) {
            write(1, "Rcvd Message : ", sizeof("Rcvd Message : "));
            nwritten = write(1, recv_buf, nbytes);
            write(1, "Type Message : ", sizeof("Type Message : "));
            memset(recv_buf, '\0', BUFFER_SIZE);
            continue;
        } else if ( nbytes < 0 ) {
            rc = SSH_ERROR;
            break;
        }

        if ( !kbhit() ) {
            usleep(5000L);
            continue;
        }

        memset(send_buf, '\0', BUFFER_SIZE);
        nbytes = read(0, send_buf, BUFFER_SIZE);

        if ( nbytes == 1 ) {
            break;
        }

        if ( nbytes > 0 ) {
            nwritten = ssh_channel_write(channel, send_buf, nbytes);
        }

    } while ( ssh_channel_is_open(channel) && !ssh_channel_is_eof(channel));

    // <-- send channel EOF to remote
    rc = ssh_channel_send_eof(channel);
    if (rc == SSH_ERROR) {
        printf("ERROR: ssh_channel_send_eof(%d) - %s",
                ssh_get_error_code(session),
                ssh_get_error(session));
    }

    // <-- waiting EOF complete
    while(!ssh_channel_is_eof(channel)) {
        memset(recv_buf, '\0', BUFFER_SIZE);
        rc = ssh_channel_read(channel, recv_buf, BUFFER_SIZE, 0);
        if (rc == SSH_ERROR) {
            printf("ERROR: ssh_channel_send_eof(%d) - %s",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
               break;
       }
    }

    if ( ssh_channel_is_open(channel) ) {
        rc = ssh_channel_close(channel);    // <-- close channel
    }
    ssh_channel_free(channel);     // <-- free channel

    // step 6 disconnect and free session
    ssh_disconnect(session);
    ssh_free(session);

    // ssh_finalize();
    return rc;
}

int main(int argc, char* argv[])
{
    int rc =0;

    if ( argc != 4 ) {
        std::cout << "Usage : " << argv[0] << " <server> <port> <login_id>" << std::endl;
        exit(EXIT_FAILURE);
    }

    struct TH_PARAM  mParam;

    memcpy(mParam.Server, argv[1], strlen(argv[1]));
    mParam.Port = std::stoi(argv[2]);
    memcpy(mParam.Login, argv[3], strlen(argv[3]));

    rc = ssh_getpass("Password : ", mParam.PassWd, sizeof(mParam.PassWd), 0, 0);
    if ( rc == SSH_OK ) {
        rc = ssh_session_function(mParam);
    }

    return rc;
}
