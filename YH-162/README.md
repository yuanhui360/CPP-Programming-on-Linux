The X Window System uses a Client/Server architecture designed to allow multiple programs to share hardware resources like keyboards, mice, 
and displays. It effectively acts as a 'translator' or bridge between a graphical application and the display server.

A key feature is X11 Forwarding, which allows you to run an application on a remote server (such as a Linux machine in a data center) while 
interacting with its window directly on your local desktop.

In this example, it will discuss libX11, a fundamental C/C++ library used for handling the Linux X Window System. Often referred to as Xlib, 
it serves as the primary client-side library for X11. and it will write a C++ graphical program (similar to xclock) that runs on a Linux 
terminal using the X11 library.

The video linke of this example : https://youtu.be/-MP7M1P0msk
