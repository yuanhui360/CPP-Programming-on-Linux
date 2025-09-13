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
        return -1;
    }

    // step 5 Loop exec command on remote
    for ( int i = 0; i < (int)inParam.cmdVec.size(); i++ ) {

        printf("command[%d] %s@%s:%d %s\n", i, inParam.Login, inParam.Server, inParam.Port,
                inParam.cmdVec[i].c_str());
        ssh_channel channel = ssh_channel_new(session);  // <-- Allocate a new channel. NULL on error
        if ( channel == NULL ) {
            printf("ERROR: ssh_channel_new(%d) - %s",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
            continue;
        }
        rc = ssh_channel_open_session(channel);          // <-- channel open new sub-session
        if ( rc == SSH_OK ) {
            printf("command[%d] -- ssh_channel_open_session() Success\n", i);
        } else {
            printf("ERROR: ssh_channel_open_session(%d) - %s",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
            continue;    // <-- Continue for next cmd
        }

        // send remote command request
        rc = ssh_channel_request_exec(channel, inParam.cmdVec[i].c_str());
        if (rc == SSH_ERROR) {
            printf("ERROR: ssh_channel_request_exec(%d) - %s",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
            continue; // <-- Continue for next cmd
        }

        char rdBuffer[256];
        int  nbytes;
        do {
            memset(rdBuffer, '\0', sizeof(rdBuffer));
            nbytes = ssh_channel_read(channel, rdBuffer, sizeof(rdBuffer), 0);
            printf("%s", rdBuffer);
        } while ( nbytes > 0 );

        if ( nbytes < 0 ) {
            printf("ERROR: ssh_channel_read(%d) - %s",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
        }

        rc = ssh_channel_send_eof(channel); // <-- send channel EOF to remote
        if (rc == SSH_ERROR) {
            printf("ERROR: ssh_channel_send_eof(%d) - %s",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
            continue; // <-- Continue for next cmd
        }

        while(!ssh_channel_is_eof(channel)) {     // <-- waiting EOF complete
            memset(rdBuffer, '\0', sizeof(rdBuffer));
            rc = ssh_channel_read(channel, rdBuffer, sizeof(rdBuffer), 0);
            if (rc == SSH_ERROR) {
                printf("ERROR: ssh_channel_send_eof(%d) - %s",
                        ssh_get_error_code(session),
                        ssh_get_error(session));
                break;
            }
        }

        rc = ssh_channel_close(channel);    // <-- close channel
        if ( rc == SSH_OK ) {
            printf("command[%d] -- ssh_channel_close() Success\n", i);
        } else {
            printf("ERROR: ssh_channel_close(%d) - %s",
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
