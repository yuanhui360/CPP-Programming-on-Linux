Libev is a full-featured, high-performance event loop libraries written in C, in general it is a C library and another widely used event 
loop library in the industry. The example in previous video, it is more looks like a C API and usually it works fine to compile a C API 
using C++ compiler.

libev comes with a few simple C++ wrapper classes that offer convenient methods for starting and stopping watchers. They also update the callback 
model to support class method callbacks directly on objects.  In this short video, it will build a File Monitor program using
these libev C++ wrapper classes.

The example in this short video, it will use libev's ev_stat and ev_timer watchers to monitor Linux file updates using the C++ wrapper classes. 
Additionally, we will include an ev_signal watcher in this example to handle and receive SIGINT signals.

Video link for this example : https://youtu.be/mYoqrcvLwk8
