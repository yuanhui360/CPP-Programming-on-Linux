#include <iostream>
#include <sys/statvfs.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <libssh/libsshpp.hpp>
#include "ssh_cpp.hpp"
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

int ssh_session_function( const struct TH_PARAM& inParam )
{
    int rc = 0;
    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];
    int nbytes, nwritten;
    memset(recv_buf, '\0', BUFFER_SIZE);
    memset(send_buf, '\0', BUFFER_SIZE);
    (void)nbytes;
    (void)nwritten;

    // step 1 start a new SSH session Object
    ssh::my_session    session_obj;

    try {
        // step 2 setup session options
        session_obj.setOption(SSH_OPTIONS_HOST, inParam.Server);
        session_obj.setOption(SSH_OPTIONS_PORT, inParam.Port);
        session_obj.setOption(SSH_OPTIONS_USER, inParam.Login);
        session_obj.setOption(SSH_OPTIONS_LOG_VERBOSITY, SSH_LOG_NOLOG);

        // step 3 session connect to remote
        session_obj.connect();

        // step 4 authenticate login/password
        session_obj.userauthPassword(inParam.PassWd);

        // step 5 Allocate a new channel. NULL on error
        ssh::Channel    channel_obj(session_obj);

       // step  6 channel open new sub-session
        channel_obj.openSession();

        // Step 7 Optionally Request a Pseudo-Terminal suing :
        //     channel.requestPty(NULL, 80, 24);
        //     channel.changePtySize(80, 24);
        //


        channel_obj.requestPty();
        channel_obj.changePtySize(80, 24);

        // Step 8 channel request remote shell
        channel_obj.requestShell();
        struct termios  terminal_local;
        session_obj.cfmakeraw(&terminal_local);

        do {
            nbytes = channel_obj.readNonblocking(recv_buf, BUFFER_SIZE);
            if ( nbytes > 0 ) {
                nwritten = write(1, recv_buf, nbytes);
                memset(recv_buf, '\0', BUFFER_SIZE);
                continue;
            } else if ( nbytes < 0 ) {
                rc = SSH_ERROR;
                break;
            }

            if ( !session_obj.kbhit() ) {
                usleep(5000L);
                continue;
            }

            // step 8 type in command into buffer for sending to remote shell
            // Step 9 If has command
                // Step 9-1 write command to remote shell
                // step 9-2 loop read response from remote shell

            memset(send_buf, '\0', BUFFER_SIZE);
            nbytes = read(0, send_buf, BUFFER_SIZE);
            if ( nbytes > 0 ) {
                nwritten = channel_obj.write(send_buf, nbytes);
            }

        } while ( channel_obj.isOpen() && !channel_obj.isEof() );

        channel_obj.sendEof();
        while ( ! channel_obj.isEof() ) {
            usleep(5000L);
        }
        if ( channel_obj.isOpen() ) {
            channel_obj.close();
        }


    } catch ( ssh::SshException c) {
        printf("SSH Exception (%d) - %s\n", c.getCode(), c.getError().c_str());
        return SSH_ERROR;
    } catch ( const std::exception &e ) {
        printf("SSH Exception - %s\n", e.what());
        return SSH_ERROR;
    }

    session_obj.disconnect();
    return rc;
}

int main(int argc, char* argv[])
{
    int rc =0;

    if ( argc != 4 ) {
        std::cout << "Usage : " << argv[0] << " <server> <port> <login_id>" << std::endl;
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
