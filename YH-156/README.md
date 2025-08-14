The crucial concept of non-blocking I/O with TLS/SSL, enhancing responsiveness and efficiency in secure applications. 
In this example, it will apply that powerful paradigm to another essential secure protocol:
SSH – the Secure Shell Protocol

libssh is a C library that enables you to write a program that uses the SSH protocol. With it, you can remotely execute programs, transfer files, 
or use a secure and transparent tunnel for your remote programs.

The most popular Usage of SSH:
1) For login to a shell on a remote host
2) For executing a command on a remote host
3) For development on a mobile or embedded device that supports SSH.
4) For securing file transfer protocols. SFTP
5) For secure copy Protocol SCP
6) For securely mounting a directory on a remote server as a filesystem on a
   local computer using SSHFS.

As we know, traditional blocking operations can halt your application's flow, especially in interactive or high-performance scenarios.  
Instead non-blocking SSH involves managing I/O operations asynchronously. Instead of waiting for a read or write to complete, your application 
can perform other tasks and check back on the SSH operation later.

Previous Video related to Libssh:
YH-55 :  SSH Client (Execute command on Remote)  --  https://youtu.be/fI6f8VbSmFQ
Video Link of this example :
YH-156 : Libssh Non Blocking I/O Concept -- https://youtu.be/ZZaEALzex30
