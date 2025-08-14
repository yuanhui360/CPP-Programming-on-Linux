#include <iostream>
#include <sys/statvfs.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <libssh/libssh.h>
#include <vector>

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

static void waiting_socket_read_activity(socket_t sockfd)
{
    /* Using select system call monitor socket
     * wait until the socket is readable
     */

    fd_set    fds;             /* A struct type of fd sets */
    int nfds;                  /* the highest numberes fd, plus 1 */
    nfds = sockfd + 1;

    FD_ZERO(&fds);            /* macro clear (removes all fd from) set */
    FD_SET(sockfd, &fds);     /* macro adds the fd to set */

    select(nfds, &fds, NULL, NULL, NULL);

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

    // if session is blocking mode, set to non-blocking mode
    if ( ssh_is_blocking(session) == 1 ) {
        ssh_set_blocking(session, 0);
    }

    // step 3 session connect to remote
    do {
        rc = ssh_connect(session);
        if ( rc == SSH_OK ) {
            break;
        } else if ( rc == SSH_AGAIN ) {
            printf("INFO: Retrying ssh_connect(%d) - %s\n", ssh_get_error_code(session), ssh_get_error(session));
            waiting_socket_read_activity(ssh_get_fd(session));
            continue;
        }
        // SSH_ERROR
    } while ( rc == SSH_AGAIN );

    if (rc == SSH_ERROR)
    {
        printf("ERROR: ssh_connect(%d) - %s\n", ssh_get_error_code(session), ssh_get_error(session));
        return -1;
    }

    // step 4 authenticate login/password
    do {
        rc = ssh_userauth_password(session, inParam.Login, inParam.PassWd);
        if ( rc == SSH_AUTH_SUCCESS ) {
            break;
        } else if ( rc == SSH_AUTH_AGAIN ) {
            printf("INFO: Retrying ssh_userauth_password(%d) - %s\n", ssh_get_error_code(session), ssh_get_error(session));
            waiting_socket_read_activity(ssh_get_fd(session));
            continue;
        }
        // For this demo, rest returns are all considering as errors
    } while ( rc == SSH_AUTH_AGAIN );

    if (rc == SSH_AUTH_ERROR)
    {
        printf("ERROR: ssh_userauth_password(%d) - %s\n", ssh_get_error_code(session), ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return -1;
    }

    // step 5 Loop exec command on remote
    for ( int i = 0; i < (int)inParam.cmdVec.size(); i++ ) {

        printf("command[%d] %s@%s:%d %s\n", i, inParam.Login, inParam.Server, inParam.Port,
                inParam.cmdVec[i].c_str());
        ssh_channel channel = ssh_channel_new(session);  // <-- Allocate a new channel. NULL on error
        if ( channel == NULL ) {
            printf("ERROR: ssh_channel_new(%d) - %s\n",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
            continue;
        }

        do {
            rc = ssh_channel_open_session(channel);          // <-- channel open new sub-session
            if ( rc == SSH_OK ) {
                printf("command[%d] -- ssh_channel_open_session() Success\n", i);
                break;
            } else if ( rc == SSH_AGAIN ) {
                printf("INFO: Retrying ssh_channel_open_session(%d) - %s\n",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
                waiting_socket_read_activity(ssh_get_fd(session));
                continue;
            }
            // SSH_ERROR
        } while ( rc == SSH_AGAIN );
        if ( rc == SSH_ERROR ) {
            printf("ERROR: ssh_channel_open_session(%d) - %s\n",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
            continue;    // <-- Continue for next cmd
        }

        // send remote command request
        do {
            rc = ssh_channel_request_exec(channel, inParam.cmdVec[i].c_str());
            if ( rc == SSH_OK ) {
                break;
            } else if ( rc == SSH_AGAIN ) {
                printf("INFO: Retrying ssh_channel_request_exec(%d) - %s\n",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
                waiting_socket_read_activity(ssh_get_fd(session));
                continue;
            }
            // SSH_ERROR
        } while ( rc == SSH_AGAIN );
        if (rc == SSH_ERROR) {
            printf("ERROR: ssh_channel_request_exec(%d) - %s\n",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
            continue; // <-- Continue for next cmd
        }

        char rdBuffer[256];
        int  nbytes;
        int timeout = 5000L;
        do {
            memset(rdBuffer, '\0', sizeof(rdBuffer));
            nbytes = ssh_channel_read_nonblocking(channel, rdBuffer, sizeof(rdBuffer), 0);
            if ( nbytes == SSH_ERROR ) {
            } else if ( nbytes > 0 ) {
                printf("%s", rdBuffer);
                timeout = 5000L;
            } else if ( nbytes == SSH_EOF ) {
                break;
            } else if (nbytes == SSH_AGAIN ) {
                printf("INFO: Retrying ssh_channel_read_nonblocking(%d) - %s\n",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
                waiting_socket_read_activity(ssh_get_fd(session));
                continue;
            }
            usleep(100 * 1000);
            timeout -= 100;
        } while ( !ssh_channel_is_eof(channel) && timeout > 0 );

        if ( nbytes < 0 ) {
            printf("ERROR: ssh_channel_read(%d) - %s\n",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
        }

        rc = ssh_channel_send_eof(channel); // <-- send channel EOF to remote
        if (rc == SSH_ERROR) {
            printf("ERROR: ssh_channel_send_eof(%d) - %s\n",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
            continue; // <-- Continue for next cmd
        }

        do {
            memset(rdBuffer, '\0', sizeof(rdBuffer));
            rc = ssh_channel_read_nonblocking(channel, rdBuffer, sizeof(rdBuffer), 0);
            if ( rc > 0 ) {
                continue;
            } else if ( rc == SSH_EOF ) {
                break;
            } else if ( rc == SSH_AGAIN ) {
                printf("INFO: Retrying ssh_channel_send_eof(%d) - %s\n",
                        ssh_get_error_code(session),
                        ssh_get_error(session));
                waiting_socket_read_activity(ssh_get_fd(session));
                continue;
            } else if ( rc == SSH_ERROR ) {
                printf("ERROR: ssh_channel_send_eof(%d) - %s\n",
                        ssh_get_error_code(session),
                        ssh_get_error(session));
                break;
            }
        } while ( !ssh_channel_is_eof(channel) );

        rc = ssh_channel_close(channel);    // <-- close channel
        if ( rc == SSH_OK ) {
            printf("command[%d] -- ssh_channel_close() Success\n", i);
        } else {
            printf("ERROR: ssh_channel_close(%d) - %s\n",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
            continue; // <-- Continue for next cmd
        }

        ssh_channel_free(channel);     // <-- free channel
        printf("command[%d] -- end process\n", i);
    }

    // step 6 disconnect and free session
    ssh_disconnect(session);
    ssh_free(session);

    // ssh_finalize();
    return rc;
}

int main(int argc, char* argv[])
{
    int rc =0;

    if ( argc < 5 ) {
        std::cout << "Usage : " << argv[0] << " <server> <port> <login_id> <command>..." << std::endl;
        exit(-1);
    }

    struct TH_PARAM  mParam;

    memcpy(mParam.Server, argv[1], strlen(argv[1]));
    mParam.Port = std::stoi(argv[2]);
    memcpy(mParam.Login, argv[3], strlen(argv[3]));

    for (int i=4; i<=argc-1; i++) {
        mParam.cmdVec.push_back(std::string(argv[i]));
    }

    rc = ssh_getpass("Password : ", mParam.PassWd, sizeof(mParam.PassWd), 0, 0);
    if ( rc == SSH_OK ) {
        rc = ssh_session_function(mParam);
    }

    mParam.cmdVec.clear();
    return rc;
}
