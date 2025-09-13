In This example, it will discuss the libssh C++ wrapper, an object-oriented interface to its C functions.
Libssh is a powerful and widely used SSH library in industry. It's primarily a C library and C API, 
the labssh C++ wrapper provide header-only embedded C++ interface and make it easy to manage session and 
channel in your C++ classes.

It's Header-only implementation design means you only need to include the relevant .hpp file in your C++ program.  
The primary reason for the header-only approach is to easier to maintain Application Binary Interface (ABI) 
compatibility and stability. In this way, when you compile your program with these headers, you will only link 
to the C version of libssh which will be kept ABI compatible. No need to recompile your C++ program each time a 
new binary-compatible version of libssh is released

Video of this example : https://youtu.be/LeK6El4oZMY

