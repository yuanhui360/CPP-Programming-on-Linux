#include <iostream>
#include <sys/statvfs.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

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
 * Setting the local terminal to "raw" mode with the cfmakeraw(3) function.
 * cfmakeraw() is a standard function under Linux:
 * 1) the echo from the keys pressed should be done by the remote side.
 * 2) User's input should not be sent once "Enter" key is pressed,
 *    but immediately after each key is pressed.
 */

void cfmakeraw(struct termios *termios_p)
{
    termios_p->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    termios_p->c_oflag &= ~OPOST;
    termios_p->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    termios_p->c_cflag &= ~(CSIZE|PARENB);
    termios_p->c_cflag |= CS8;
}

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
        return SSH_ERROR;
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
        return SSH_ERROR;
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
    rc = ssh_channel_open_session(channel);          // <-- channel open new sub-session
    if ( rc == SSH_ERROR ) {
        ssh_channel_free(channel);
        printf("ERROR: ssh_channel_open_session(%d) - %s",
                ssh_get_error_code(session),
                ssh_get_error(session));
        return SSH_ERROR;
    }

    // Step 7 Optionally Request a Pseudo-Terminal suing :
    //     ssh_channel_request_pty(channel);
    //     ssh_channel_change_pty_size(channel, 80, 24);
    //
    rc = ssh_channel_request_pty(channel);
    if ( rc == SSH_ERROR ) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        printf("ERROR: ssh_channel_request_pty(%d) - %s",
                ssh_get_error_code(session),
                ssh_get_error(session));
        return SSH_ERROR;
    }

    rc = ssh_channel_change_pty_size(channel, 80, 24);
    if ( rc == SSH_ERROR ) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        printf("ERROR: ssh_channel_change_pty_size(%d) - %s",
                ssh_get_error_code(session),
                ssh_get_error(session));
        return SSH_ERROR;
    }

    // Step 8 channel request remote shell
    rc = ssh_channel_request_shell(channel);
    if ( rc == SSH_ERROR ) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        printf("ERROR: ssh_channel_request_shell(%d) - %s",
                ssh_get_error_code(session),
                ssh_get_error(session));
        return SSH_ERROR;
    }
    // Step 9 set local terminal raw mode with the cfmakeraw(3) function.
    //        https://linux.die.net/man/3/cfmakeraw   <-- Linux function

    struct termios terminal_local;
    cfmakeraw(&terminal_local);

    // Step 10 Loop Interactive Remote Shell Session

    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];
    memset(recv_buf, '\0', BUFFER_SIZE);
    memset(send_buf, '\0', BUFFER_SIZE);
    int nbytes, nwritten;
    (void) nbytes;
    (void) nwritten;

    do {
        nbytes = ssh_channel_read_nonblocking(channel, recv_buf, BUFFER_SIZE, 0);
        if ( nbytes > 0 ) {
            nwritten = write(1, recv_buf, nbytes);
            memset(recv_buf, '\0', BUFFER_SIZE);
            continue;
        } else if ( nbytes < 0 ) {
            rc = SSH_ERROR;
        }

        if ( !kbhit() ) {
            usleep(5000L);
            continue;
        }

        memset(send_buf, '\0', BUFFER_SIZE);
        nbytes = read(0, send_buf, BUFFER_SIZE);
        if ( nbytes > 0 ) {
            nwritten = ssh_channel_write(channel, send_buf, nbytes);
        }

    } while ( ssh_channel_is_open(channel) && !ssh_channel_is_eof(channel));

    // Step 11 send channel eof, close and free channel
    ssh_channel_send_eof(channel);
    while ( !ssh_channel_is_eof(channel)) {
        usleep(1000L);
    }

    if ( ssh_channel_is_open(channel)) {
        ssh_channel_close(channel);
    }
    ssh_channel_free(channel);

    // step 12 disconnect and free session
    ssh_disconnect(session);
    ssh_free(session);

    // ssh_finalize();
    return rc;
}

int main(int argc, char* argv[])
{
    int rc =0;

    if ( argc != 4 ) {
        std::cout << "Usage : " << argv[0] << " <server> <port> <login_id> " << std::endl;
        exit(-1);
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
