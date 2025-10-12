This example, it will discuss another popular usage of libssh: opening a remote shell.

Comparison
Comparison of previous video's example Executing a Command on Remote Host vs. Opening a Remote Shell, Both processes allow you to execute a command on a remote host and read the response.

The key difference are:
The process of requesting to execute a command on a remote host is more like a backend batch shell, whereas opening a remote shell is much more useful for an interactive shell application.

Opening a remote shell using libssh involves several steps, building upon an established SSH session.
1) Establish an SSH Session:
  - Initialize an ssh_session object using ssh_new().
  - Set connection options (e.g., hostname, port, verbosity) using ssh_options_set().
  - Connect to the remote host with ssh_connect().
  - Authenticate the client (e.g., using password or public key) with functions like
    ssh_userauth_password() or ssh_userauth_publickey_auto().
2) Create a Channel:
  - Allocate a new channel using ssh_channel_new(session). This creates a logical
    connection within the established SSH session.
3) Open a Session on the Channel:
  - Open a session on the created channel using ssh_channel_open_session(channel).
    This prepares the channel for specific operations like requesting a shell or
    executing a command.
4) Request a Pseudo-Terminal (Optional but Recommended for Interactive Shells):
  - If an interactive shell is desired, request a pseudo-terminal (pty) using
    ssh_channel_request_pty(channel). This simulates a terminal environment on
    the remote server, enabling interactive input and output.
  - Set the terminal type and size using ssh_channel_request_pty_size(channel,
    "xterm", 80, 24).
5) Request a Shell:
  - Request a remote shell on the channel using ssh_channel_request_shell(channel).
6) Interact with the Shell:
  - Read output from the remote shell using ssh_channel_read(channel, buffer, size, 0).
  - Write input to the remote shell using ssh_channel_write(channel, buffer, size).
7) Close and Free Resources:
  - When finished, send an end-of-file to the channel using ssh_channel_send_eof(channel).
  - Close the channel using ssh_channel_close(channel).
  - Free the channel resources using ssh_channel_free(channel).
  - Disconnect the SSH session using ssh_disconnect(session).
  - Free the SSH session resources using ssh_free(session).

The example video link : https://youtu.be/i5GsY9rDvjM
