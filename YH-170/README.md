This example It's going to discuss another popular C++ client library for RabbitMQ.  AMQP-CPPA is an asynchronous, non-blocking C++ library 
designed for RabbitMQ. It handles parsing incoming data and generating frames to send to the RabbitMQ server.

This library has a layered architecture,  It allows you—if you prefer—to fully manage the network layer yourself. This layered architecture 
makes the library flexible and portable: it does not necessarily rely on operating system specific IO calls, and can be easily integrated into 
any kind of event loop.

AMQP-CPP is fully asynchronous and does not do any blocking (system) calls, so it can be used in high performance applications without the need 
for threads. Also, this library requires compiler to support C++ 17.

Video link of this example : https://youtu.be/ORXZQ4LZB6A
