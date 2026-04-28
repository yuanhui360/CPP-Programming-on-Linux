This example will discuss XCB (X C Binding), an alternative to libX11. As a low-level API for the X Window System, XCB is the current recommendation 
from X.org for new development. However, this does not mean X11 (libX11) is deprecated. Many existing APIs are still built on top of libX11, and because it 
remains deeply integrated into most X Window applications, it is difficult to replace entirely.

The X C Binding (XCB) was designed to address several limitations in the original Xlib, providing a more modern foundation for:
  1) Lightweight Platforms: XCB is much smaller and more modular, making it ideal for embedded   or resource-constrained systems.
  2) Latency Hiding: XCB allows for asynchronous requests, letting the application   continue working until the result is actually needed.
  3) Direct Protocol Access: XCB provides a transparent, one-to-one mapping to the X protocol.
  4) Threaded Applications: XCB was built from the ground up with thread safety and   modern concurrency in mind.
  5) Extensibility: XCB support for new X extensions and make it much simpler   and more reliable.

Video link of this example : https://youtu.be/M0LaL_DFrNE
