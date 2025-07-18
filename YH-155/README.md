This example will discuss on the non-blocking concept of TLS/SSL using wolfSSL library,
explained in simple terms.
In blocking mode, a TLS operation (like a handshake or encryption) pauses the
application's execution until the operation is fully completed.
In non-blocking mode, network operations return immediately, even if they haven't finished.
The application then checks the error code to determine whether to retry, proceed, or failure.

The benefits of Non-Blocking TLS include:
1) Improved Performance
2) Better Responsiveness
3) Suitability for Resource-Constrained Environments
4) Flexibility in I/O and Threading Models

Here is link of reference documentation site:
https://www.wolfssl.com/documentation/manuals/wolfssl/index.html

Here are video link of this example : https://youtu.be/s_mBaH9FH4g

