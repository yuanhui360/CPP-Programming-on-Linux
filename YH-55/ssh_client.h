#ifndef SSH_CLIENT_H
#define SSH_CLIENT_H

#include <iostream>
#include <sys/statvfs.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <libssh/libssh.h>
#include <libssh/libsshpp.hpp>
#include <libssh/sftp.h>


int run_remote_cmd(ssh_session session, char* inCmd)
{
  ssh_channel channel;
  int rc;
  char buffer[256];
  int nbytes;

  // step 1 start new channel
  channel = ssh_channel_new(session);
  // step 2 channel open new sub-session
  rc = ssh_channel_open_session(channel);
  // step 3 send remote command request
  rc = ssh_channel_request_exec(channel, inCmd);

  // step 4 read remote response back
  nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  while (nbytes > 0)
  {
      std::cout << buffer << std::endl;
      nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  }

  // step 5 send channel EOF to remote
  ssh_channel_send_eof(channel);
  // step 6 close channel
  ssh_channel_close(channel);
  // step 7 free channel
  ssh_channel_free(channel);

  rc = SSH_OK;
  return rc;
}

int verify_knownhost(ssh_session session)
{
    enum ssh_known_hosts_e state;
    unsigned char *hash = NULL;
    ssh_key srv_pubkey = NULL;
    size_t hlen;
    char buf[10];
    char *hexa;
    char *p;
    int cmp;
    int rc;

    rc = ssh_get_server_publickey(session, &srv_pubkey);
    if (rc < 0) {
        return -1;
    }

    rc = ssh_get_publickey_hash(srv_pubkey,
                                SSH_PUBLICKEY_HASH_SHA1,
                                &hash,
                                &hlen);
    ssh_key_free(srv_pubkey);
    if (rc < 0) {
        return -1;
    }

    state = ssh_session_is_known_server(session);
    switch (state) {
        case SSH_KNOWN_HOSTS_OK:
            /* OK */
            break;
        case SSH_KNOWN_HOSTS_CHANGED:
            std::cout << "Host key for server changed: it is now:" << std::endl;
            ssh_print_hexa("Public key hash", hash, hlen);
            std::cout << "For security reasons, connection will be stopped" << std::endl;;
            ssh_clean_pubkey_hash(&hash);
            return -1;
        case SSH_KNOWN_HOSTS_OTHER:
            std::cout << "The host key for this server was not found but an other";
            std::cout << " type of key exists.\n";
            std::cout <<  "An attacker might change the default server key to";
            std::cout <<  "confuse your client into thinking the key does not exist\n";
            ssh_clean_pubkey_hash(&hash);
            return -1;
        case SSH_KNOWN_HOSTS_NOT_FOUND:
            std::cout <<  "Could not find known host file." << std::endl;
            std::cout <<  "If you accept the host key here, the file will be ";
            std::cout <<  "automatically created.\n";

            /* FALL THROUGH to SSH_SERVER_NOT_KNOWN behavior */

        case SSH_KNOWN_HOSTS_UNKNOWN:
            hexa = ssh_get_hexa(hash, hlen);
            std::cout << "The server is unknown. Do you trust the host key?" << std::endl;
            std::cout << "Public key hash: " <<  hexa << std::endl;
            ssh_string_free_char(hexa);
            ssh_clean_pubkey_hash(&hash);
            p = fgets(buf, sizeof(buf), stdin);
            if (p == NULL) {
                return -1;
            }

            cmp = strncasecmp(buf, "yes", 3);
            if (cmp != 0) {
                return -1;
            }

            rc = ssh_session_update_known_hosts(session);
            if (rc < 0) {
                std::cout <<  "Error " <<  strerror(errno) << std::endl;
                return -1;
            }

            break;
        case SSH_KNOWN_HOSTS_ERROR:
            std::cout <<  "Error " <<  ssh_get_error(session) << std::endl;
            ssh_clean_pubkey_hash(&hash);
            return -1;
    }

    ssh_clean_pubkey_hash(&hash);
    return 0;
}


int run_remote_cmd_with_check(ssh_session session, const std::string& cmd)
{
  ssh_channel channel;
  int rc;
  char buffer[256];
  int nbytes;

  channel = ssh_channel_new(session);
  if (channel == NULL)
    return SSH_ERROR;

  rc = ssh_channel_open_session(channel);
  if (rc != SSH_OK)
  {
    ssh_channel_free(channel);
    return rc;
  }

  rc = ssh_channel_request_exec(channel, cmd.c_str());
  if (rc != SSH_OK)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return rc;
  }

  nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  while (nbytes > 0)
  {
    if (write(1, buffer, nbytes) != (unsigned int) nbytes)
    {
      ssh_channel_close(channel);
      ssh_channel_free(channel);
      return SSH_ERROR;
    }
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  }

  if (nbytes < 0)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return SSH_ERROR;
  }

  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  ssh_channel_free(channel);

  return SSH_OK;
}
#endif
