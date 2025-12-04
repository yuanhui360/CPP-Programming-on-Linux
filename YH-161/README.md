In this example, it will focus on SSH proxy server. As we know, proxy server has been widely used in network architecture. in our channel,
We've previously explored 2 videos related to proxy servers. We've built a TCP Proxy server using libevent - Bufferevents and an HTTP Proxy server using libevent- Evhttp.  
Here, We are going to create a simple SSH proxy server in C++ using the libssh library.

A proxy server is a vital intermediary, acting as a gateway between a client's device and a remote server. It forwards client requests to the destination server using its 
own IP address, providing a critical layer of anonymity, security, and network traffic control.

There are many protocols for proxy servers. The SSH Proxy server is specifically designed to manage Secure Shell connections. Its primary function is to listen for incoming 
SSH traffic, securely forward that traffic to a remote server, and then relay the remote server's response back to the client.

Below is the link of previous video related proxy server.
- YH-146, Libevent Bufferevent TCP Proxy Server - https://youtu.be/n1M10fNdbs4
- YH-147, Libevent Evhttp HTTP Proxy Server Example - https://youtu.be/8C2lex3CJ5c

Below is video link of this example.
- YH-161, Libssh SSH Data Proxy Server - https://youtu.be/vfbZb4Hz_Fw
