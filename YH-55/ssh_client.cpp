#include "ssh_client.h"
#include <unistd.h>
#include <libssh/libssh.h>

/*
 * If you are using libssh statically linked, don't forget to call ssh_init()
 * before using any of libssh provided functions (and ssh_finalize() in the end).
 */

int main(int argc, char* argv[])
{
    if ( argc != 5 ) {
        std::cout << "Usage : ssh_client <server> <port> <login_id> <command>" << std::endl;
        exit(-1);
    }

    int mPort = std::stoi(argv[2]);
    int verbosity = SSH_LOG_NOLOG;
    int rc =0;
    char sPass[20];
    char command[256];
    memset(command, '\0', sizeof(command));
    memcpy(command, argv[4], strlen(argv[4]));
    std::cout << "remote command : " << command << std::endl;

    // step 1 start a new SSH session
    ssh_session mSession = ssh_new();
    if(mSession == NULL) {
        exit(-1);
    }

    // step 2 setup session options
    ssh_options_set(mSession, SSH_OPTIONS_HOST, argv[1]);
    ssh_options_set(mSession, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(mSession, SSH_OPTIONS_PORT, &mPort);

    // step 3 session connect to remote
    rc = ssh_connect(mSession);
    if (rc != SSH_OK)
    {
       std::cout << "Error connecting to localhost: " << ssh_get_error(mSession) << std::endl;
       exit(-1);
    }

    // step 4 authenticate login/password and knownhost
    rc =  verify_knownhost(mSession);
    memset(sPass, '\0', sizeof(sPass));
    rc = ssh_getpass("Password : ", sPass, sizeof(sPass), 0, 0);
    rc = ssh_userauth_password(mSession, argv[3], sPass);
    if (rc != SSH_AUTH_SUCCESS)
    {
        std::cout <<  "Error authenticating with password: " << ssh_get_error(mSession) << std::endl;
        ssh_disconnect(mSession);
        ssh_free(mSession);
        exit(-1);
    }

    // step 5 exec command on remote
    rc =  run_remote_cmd(mSession, command);

    // step 6 disconnect and free session
    ssh_disconnect(mSession);
    ssh_free(mSession);
    return rc;
}
