#include <iostream>
#include <sys/statvfs.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <asm/ioctls.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <pty.h>

#include <libssh/libssh.h>
#include <libssh/callbacks.h>
#include <libssh/server.h>

#include <pthread.h>
#include <vector>

/* A userdata struct for API */
struct global_data
{
    int         id;
    char        Server[256];
    int         Port;
    char        Login[256];
    char        PassWd[256];
    char        rsa_key[256];
    global_data() : id(0), Port(0)
    {
        memset(Server,'\0', sizeof(Server));
        memset(Login, '\0', sizeof(Login));
        memset(PassWd, '\0', sizeof(PassWd));
        memset(rsa_key, '\0', sizeof(rsa_key));
    }
};

/* A userdata struct for session. */
struct session_data
{
    int id;
    ssh_channel channel;
    int  authenticated;
    int  auth_attempts;
    int  erase_flag;

    session_data() : id(0), channel(NULL), authenticated(0),
                     auth_attempts(0), erase_flag(0) {
    }
};

static struct global_data  gParam;
std::vector<struct session_data> vsdata;

void check_and_purge_vector()
{
    // check and purge vertor vsdata mark flaged record
    // if all records have been marked as erase, then clean vsdata

    for ( std::vector<struct session_data>::iterator it = vsdata.begin(); it != vsdata.end(); )
    {
        if ( it->erase_flag == 0 ) {
            return;
        } else {
            ++it;
        }
    }

    vsdata.clear();
}

struct session_data * search_session_data_vector(int id)
{
    struct session_data sdata;

    // search session data vector
    for ( size_t i = 0; i < vsdata.size(); i++ ) {
        if ( vsdata[i].id == id ) {
            return &vsdata[i];
        }
    }
    return NULL;
}

/*
 *  struct ssh_channel_callbacks_struct channel_cb = {
 *      .channel_data_function = my_channel_data_function_cb,
 *      .channel_eof_function = my_channel_eof_cb,
 *      .channel_close_function = my_channel_close_cb,
 *
 *  struct ssh_server_callbacks_struct server_cb = {
 *      .auth_password_function = my_auth_password_cb,
 *      .service_request_function = my_service_request_cb,
 *      .channel_open_request_session_function = my_channel_open_session_cb,
 */

int my_channel_data_function_cb (ssh_session session, ssh_channel channel, void *data, uint32_t len,
        int is_stderr, void *userdata)
{
    struct global_data *my_Param = (struct global_data *) userdata;
    (void) session;
    (void) channel;
    (void) is_stderr;
    printf("INFO: my_channel_data_function_cb(%d)\n", my_Param->id);
    struct session_data *psdata = search_session_data_vector(my_Param->id);
    if ( psdata == NULL ) {
        return SSH_ERROR;
    }

    int nbytes = len;
    printf("INFO: my_channel_data_function_cb(%d) [%d] - %s\n", my_Param->id, nbytes, (char*)data);
    ssh_channel_write(channel, (char*)data, nbytes);
    return nbytes;
}

void my_channel_eof_cb (ssh_session session, ssh_channel channel, void *userdata)
{
    struct global_data *my_Param = (struct global_data *) userdata;
    (void) session;
    printf("INFO: my_channel_eof_cb(%d)\n", my_Param->id);
    ssh_channel_send_eof(channel);
}

void my_channel_close_cb (ssh_session session, ssh_channel channel, void *userdata)
{
    struct global_data *my_Param = (struct global_data *) userdata;
    (void) session;
    printf("INFO: my_channel_close_cb(%d)\n", my_Param->id);
    ssh_channel_close(channel);
}

ssh_channel my_channel_open_session_cb (ssh_session session, void *userdata)
{
    struct global_data *my_Param = (struct global_data *) userdata;
    struct session_data *psdata = search_session_data_vector(my_Param->id);
    if ( psdata == NULL ) {
        return NULL;
    }

    psdata->channel = ssh_channel_new(session);
    if ( psdata->channel == NULL ) {
        printf("ERROR: my_channel_open_session_cb(%d)->ssh_channel_new()\n", my_Param->id);
    } else {
        printf("INFO: my_channel_open_session_cb(%d)->ssh_channel_new() - Success\n", my_Param->id);
    }
    return psdata->channel;
}

int my_service_request_cb (ssh_session session, const char *service, void *userdata)
{
    struct global_data *my_Param = (struct global_data *) userdata;
    if ( session == NULL ) {
        printf("ERROR: my_service_request_cb(%d) = session is NULL\n", my_Param->id);
        return SSH_ERROR;
    }
    printf("INFO: my_service_request_cb(%d) - %s Allowed\n", my_Param->id, service);
    return SSH_OK;
}

int my_auth_none_cb(ssh_session session, const char *user, void *userdata)
{
    (void) session;
    struct global_data *my_Param = (struct global_data *) userdata;
    printf("INFO: my_auth_none_cb(%d) - %s\n", my_Param->id, user);
    return SSH_AUTH_SUCCESS;
}

int my_auth_password_cb(ssh_session session, const char *user, const char *password, void *userdata)
{
    (void) session;
    struct global_data *my_Param = (struct global_data *) userdata;
    struct session_data *psdata = search_session_data_vector(my_Param->id);
    if ( psdata == NULL ) {
        return SSH_ERROR;
    }
    printf("INFO: my_auth_password_cb(%d) - %s\n", my_Param->id, user);
    psdata->auth_attempts++;
    if ( strcmp(user, my_Param->Login) == 0 && strcmp(password, my_Param->PassWd) == 0) {
        printf("INFO: my_auth_password_cb(%d) - %s Success\n", my_Param->id, user);
        psdata->authenticated = 1;
        return SSH_AUTH_SUCCESS;
    }

    return SSH_AUTH_DENIED;

}


void my_handle_session(struct global_data myParam, ssh_session session)
{
    // Search and locat session record in session vector
    int rc;
    size_t iVec = 0;
    for ( size_t i = 0; i < vsdata.size(); i++ ) {
        if ( vsdata[i].id == myParam.id ) {
            iVec = i;
            break;
        }
    }
    printf("INFO: my_handle_session(%d)<%d>\n", myParam.id, (int)iVec);

    // Handle the SSH session key exchange (handsshake)
    if ( ssh_handle_key_exchange(session) != SSH_OK ) {
        printf("ERROR: ssh_handle_key_exchange(%d) : %s\n",
                ssh_get_error_code(session), ssh_get_error(session));
        return;
    } else {
        printf("INFO: ssh_handle_key_exchange(%d)<%d> - Success\n", myParam.id, (int)iVec);
    }

    // create new event object my_event
    ssh_event my_event = ssh_event_new();
    if ( my_event == NULL ) {
        printf("ERROR: ssh_event_new(%d) : %s\n",
                ssh_get_error_code(session), ssh_get_error(session));
        return;
    }

    // declare and initialize ssh_channel_callbacks_struct
    struct ssh_channel_callbacks_struct channel_cb = {
        .size = 0,
        .userdata = &myParam,
        .channel_data_function = my_channel_data_function_cb,
        .channel_eof_function = my_channel_eof_cb,
        .channel_close_function = my_channel_close_cb,
        .channel_signal_function = NULL,
        .channel_exit_status_function = NULL,
        .channel_exit_signal_function = NULL,
        .channel_pty_request_function = NULL,
        .channel_shell_request_function = NULL,
        .channel_auth_agent_req_function = NULL,
        .channel_x11_req_function = NULL,
        .channel_pty_window_change_function = NULL,
        .channel_exec_request_function = NULL,
        .channel_env_request_function = NULL,
        .channel_subsystem_request_function = NULL,
        .channel_write_wontblock_function = NULL
    };

    // declare and initialize ssh_server_callbacks_struct
        // gssapi <-- stands for Generic Security Services Application Program Interface
        // mic    <-- stands for Message Integrity Check.
    struct ssh_server_callbacks_struct server_cb = {
        .size = 0,
        .userdata = &myParam,
        .auth_password_function = my_auth_password_cb,
        .auth_none_function = my_auth_none_cb,
        .auth_gssapi_mic_function = NULL,
        .auth_pubkey_function = NULL,
        .service_request_function = my_service_request_cb,
        .channel_open_request_session_function = my_channel_open_session_cb,
        .gssapi_select_oid_function = NULL,
        .gssapi_accept_sec_ctx_function = NULL,
        .gssapi_verify_mic_function = NULL
    };

    ssh_set_auth_methods(session, SSH_AUTH_METHOD_PASSWORD);
    ssh_callbacks_init(&server_cb);
    ssh_callbacks_init(&channel_cb);

    ssh_set_server_callbacks(session, &server_cb);
    ssh_event_add_session(my_event, session);

    // Loop attempts authenticate 3 times in 10 seconds (n * 100ms)
    int n = 0;
    while ( vsdata[iVec].authenticated == 0 || vsdata[iVec].channel == NULL ) {
        if ( vsdata[iVec].auth_attempts >= 3 || n >= 100) {
            ssh_event_free(my_event);
            return;
        }
        if ( ssh_event_dopoll(my_event, 100) == SSH_ERROR) {
            printf("ERROR: ssh_event_dopoll(%d) : %s\n",
                ssh_get_error_code(session), ssh_get_error(session));
            ssh_event_free(my_event);
            return;
        }
    }

    rc = ssh_set_channel_callbacks(vsdata[iVec].channel, &channel_cb);
    if ( rc == SSH_ERROR ) {
        printf("ERROR: ssh_set_channel_callbacks(%d) : %s\n",
               ssh_get_error_code(session), ssh_get_error(session));
        ssh_event_free(my_event);
        return;
    }
    printf("INFO: ssh_set_channel_callbacks(%d)<%d> = Success\n", myParam.id, (int)iVec);
    // Loop Poll event while channel is Open
    do {
        if ( ssh_event_dopoll(my_event, 100) == SSH_ERROR ) {
            printf("ERROR: ssh_set_channel_callbacks(%d) : %s\n",
               ssh_get_error_code(session), ssh_get_error(session));
        }
        usleep(1000L);
    } while ( ssh_channel_is_open(vsdata[iVec].channel));

    // free channel and free event
    ssh_channel_free(vsdata[iVec].channel);
    ssh_event_free(my_event);

}


void *my_session_thread_function(void *arg)
{
    ssh_session session = (ssh_session) arg;
    struct global_data myParam;

    // copy global parameters to local myParam
    myParam.id = gParam.id;
    myParam.Port = gParam.Port;
    memcpy(myParam.Server, gParam.Server, sizeof(gParam.Server));
    memcpy(myParam.Login, gParam.Login, strlen(gParam.Login));
    memcpy(myParam.rsa_key, gParam.rsa_key, strlen(gParam.rsa_key));
    memcpy(myParam.PassWd, gParam.PassWd, sizeof(gParam.PassWd));

    struct session_data sdata;
    sdata.id = myParam.id;
    vsdata.push_back(sdata);

    my_handle_session(myParam, session);
    for ( size_t i = 0; i < vsdata.size(); i++ ) {
        if ( vsdata[i].id == myParam.id ) {
            printf("INFO: my_session_thread_function(%d)<%d> - Mark Erase\n", myParam.id, (int)i);
            vsdata[i].erase_flag = 1;
            break;
        }
    }

    ssh_disconnect(session);
    ssh_free(session);
    return NULL;
}

int main(int argc, char* argv[])
{
    int rc =0;
    ssh_bind    sshbind;
    ssh_session session;
    (void) sshbind;
    (void) session;

    if ( argc != 6 ) {
        std::cout << "Usage : " << argv[0] << " <uri> <port> <login> <id_rsa> <log Level [0-3]>" << std::
        std::cout << "Example : " << std::endl;
        std::cout << "        " << argv[0] << " 10.0.0.191 1234 hyuan /home/hyuan/.ssh/id_rsa 0" << std::
        exit(-1);
    }

    memcpy(gParam.Server, argv[1], strlen(argv[1]));
    gParam.Port = std::stoi(argv[2]);
    memcpy(gParam.Login, argv[3], strlen(argv[3]));
    memcpy(gParam.rsa_key, argv[4], strlen(argv[4]));

    rc = ssh_getpass("Password : ", gParam.PassWd, sizeof(gParam.PassWd), 0, 0);
    if ( rc != SSH_OK ) {
        printf("ERROR: ssh_getpass failed\n");
        return -1;
    }

    //  Main Server process:
    //     1) Initialize global ssh data structures
    rc = ssh_init();        // <-- Initialize global cryptographic data structures
    if (rc == SSH_ERROR ) {
        printf("ERROR: ssh_init() failed\n");
        return -1;
    }

    //     2) Create SSH bind object
    sshbind = ssh_bind_new();
    if ( sshbind == NULL ) {
        printf("ERROR: ssh_bind_new() failed\n");
        ssh_finalize();
        return -1;
    }

    //     3) Set server options
    rc = ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDADDR, argv[1]);
    rc = ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT_STR, argv[2]);
    rc = ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, argv[4]);
    rc = ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_LOG_VERBOSITY_STR, argv[5]);

    //     4) Start listening
    rc = ssh_bind_listen(sshbind);
    if ( rc < 0 ) {
        printf("ERROR: ssh_bind_listen(%d) - %s\n", rc, ssh_get_error(sshbind));
        ssh_bind_free(sshbind);
        ssh_finalize();
        return -1;
    }
    printf("INFO: ssh_bind_listen() on %s:%s\n", argv[1], argv[2]);


    //     5) LOOP
    while (1) {

        check_and_purge_vector();

        // 5-1) New Session <-- ssh_new()
        session = ssh_new();
        if ( session == NULL ) {
            printf("ERROR: ssh_new() failed");
            usleep(1000L);
            continue;
        }

        // 5-2) Bind Accept <-- ssh_bind_accept()
        rc = ssh_bind_accept(sshbind, session);
        if ( rc == SSH_ERROR ) {
            printf("ERROR: ssh_bind_accept(%d) - %s\n",
                    ssh_get_error_code(session),
                    ssh_get_error(session));
            ssh_disconnect(session);
            ssh_free(session);
            continue;
        }

        // 5-3) create thread handling session, channel function
        pthread_t tid;
        gParam.id++;
        rc = pthread_create(&tid, NULL, my_session_thread_function, session);
        if ( rc == 0 ) {
            pthread_detach(tid);
            continue;
        }
    }

    //    6) closing process
    //           disconnect and free sessio, freen network bind
    ssh_disconnect(session);
    ssh_free(session);
    ssh_bind_free(sshbind);

    //    7) finalize and clean up akk data structure
    rc = ssh_finalize();    // <-- Finalize and clean up all data structures
    if ( rc == 1 ) {
        printf("ERROR : ssh_finalize failed\n");
        rc = -1;
    }
    return rc;
}
