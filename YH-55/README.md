SSH - stands for Secure Shell
The Secure Shell Protocol (SSH) is a cryptographic network protocol for operating network services securely over an unsecured network.[1] Its most notable applications are remote login and command-line execution.
SSH applications are based on a client–server architecture, connecting an SSH client instance with an SSH server.

The example code demonstrated how to write a C++ program to build a SSH client to execute command on remote server.
The usage of ssh_client will be: 

* Usage : ssh_client <server> <port> <login_id> <command>
* For example: ssh_client 172.0.0.1 22 login_id "ls -l *.txt"

The video link of this example : https://youtu.be/fI6f8VbSmFQ

Note: There is a improvement of the example code uploaded here, usage of example here will take remoste-command from command line 
argument instead in video the remote-command is hard coded in remote execute function.
