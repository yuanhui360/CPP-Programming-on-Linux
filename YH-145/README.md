This example will discuss Bufferevents: concepts and basics, another topic of using libevent library
https://libevent.org/libevent-book/Ref6_bufferevent.html

A "bufferevent" consists of an underlying transport (like a socket), each bufferevent has a read buffer, and a write buffer. Instead of regular event, 
bufferevent invoke callbacks when data is ready to be read or written. Every bufferevent has two data-related callbacks: a read callback and a write callback. and one event related callback.
By default, the read callback is called whenever any data is read from the underlying transport, 
and the write callback is called whenever enough data from the output buffer is emptied to the underlying transport. 
It is possible to override the behavior of these functions by adjusting the read and write "watermarks" of the bufferevent.

The example of this video, it will write a TCP echo server C++ program using libevent, bufferevent concept, and then upgrade it to SSL/TLS echo server.

As we know there are many ways or technologies to write TCP application, in this channel,  we have created few videos wrote TCP echo server program using different library, or technologies.  wish they are helpful for your reference.
The video link for thsi example : https://youtu.be/MC7b5nvGmBc

