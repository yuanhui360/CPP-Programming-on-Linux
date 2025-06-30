This example will focus on the concept of TLS/SSL non-blocking mode. We will demonstrate how a TLS/SSL non-blocking client works by writing 
a C++ program example using the OpenSSL library.

1. What is Blocking?
In a blocking scenario, a TLS operation (e.g., handshake, encryption) would halt the application's execution until the operation completes.
This can lead to performance issues and a non-responsive user interface.

3. What is Non-Blocking?
In a nonblocking application you will need work to perform in the event that want to read or
want to write to the socket, but it's currently unable to do, and then wait until socket is
ready.  Non-blocking TLS utilizes techniques like:
Asynchronous operations:
Cryptographic tasks are broken down into smaller parts, and the application can proceed
with other work while waiting for the completion of these smaller parts

4. Benefits of Non-Blocking TLS:
3-1) Improved performance:
     The application can continue to process other tasks concurrently with the TLS operations,
     reducing latency and improving overall performance.
3-2) Better responsiveness:
     The application can remain responsive to user input and other events while waiting for TLS
     operations to complete.
3-3) Suitable for resource-constrained environments:
     In embedded systems or mobile devices, non-blocking TLS can be crucial for maximizing
     performance and minimizing resource consumption.
3-4) Flexibility in I/O and threading models:
     The SSLEngine class in Java SE allows applications to use non-blocking I/O channels
     and different threading models for TLS operations.

OpenSSL command for create certificate and private key :
openssl req -x509 -sha256 -nodes -newkey rsa:4096 -keyout my_private_key.pem -days 730 -out my_certificate.pem

The video link of this example : https://youtu.be/7-FyscjxVvk
