#include <iostream>
#include <sys/statvfs.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <libssh/libsshpp.hpp>
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

    try {
        // step 1 start a new SSH session Object
        ssh::Session    session_obj;

        // step 2 setup session options
        session_obj.setOption(SSH_OPTIONS_HOST, inParam.Server);
        session_obj.setOption(SSH_OPTIONS_PORT, inParam.Port);
        session_obj.setOption(SSH_OPTIONS_USER, inParam.Login);
        session_obj.setOption(SSH_OPTIONS_LOG_VERBOSITY, SSH_LOG_NOLOG);

        // step 3 session connect to remote
        session_obj.connect();

        // step 4 authenticate login/password
        session_obj.userauthPassword(inParam.PassWd);

        // step 5 Loop exec command on remote
        for ( int i = 0; i < (int) inParam.cmdVec.size(); i++ ) {

            // <step  6 print command to execute
            printf("command [%d] -- %s\n", i, inParam.cmdVec[i].c_str());

            // <step  7 Allocate a new channel. NULL on error
            ssh::Channel    channel_obj(session_obj);

            // <step  8 channel open new sub-session
            channel_obj.openSession();

            // <step  9 send remote command request
            channel_obj.requestExec(inParam.cmdVec[i].c_str());

            // <step 10 print remote response
            std::vector<char>    buffer(256);
            int nbytes;
            int tot_bytes = 0;
            do {
                nbytes = channel_obj.read(buffer.data(), buffer.size(), false);
                std::cout.write(buffer.data(), nbytes);
                tot_bytes += nbytes;
            } while ( nbytes > 0 );
            if ( nbytes < 0 ) {
                printf("command [%d] -- ERROR : channel_obj.read(%d) - %s\n", i, session_obj.getErrorCode(),
                        session_obj.getError());
            } else {
                printf("command [%d] -- SUCCESS : channel_obj.read(%d bytes)\n", i, tot_bytes);
            }

            // <step 11 send and waiting channel EOF
            channel_obj.sendEof();
            int timeout = 5000L;
            while ( !channel_obj.isEof() && timeout > 0 ) {
                usleep ( 1000L );
                timeout -= 100;
            }
            // <step 12 close channel
            if ( channel_obj.isOpen() ) {
                channel_obj.close();
            }

            //          loop for next cmd>
        }

    } catch ( ssh::SshException c) {
        printf("SSH Exception (%d) - %s\n", c.getCode(), c.getError().c_str());
        return -1;
    } catch ( const std::exception &e ) {
        printf("SSH Exception - %s\n", e.what());
        return -1;
    }

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
