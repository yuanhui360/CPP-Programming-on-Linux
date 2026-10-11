Libev is a full-featured, high-performance asynchronous event loop library written in C.
Loosely modeled after libevent, it provides a highly optimized implementation designed to
scale efficiently while maintaining a tiny memory and code footprint
Key Features
Multi-Backend Support:
    It abstracts OS-specific polling mechanisms, automatically utilizing highly optimized
    backends like Linux epoll, BSD kqueue, Solaris event ports, poll, and select
Diverse Watchers:
    It manages various event types through specialized structures called watchers:
        ev_io: Monitors file descriptors and sockets for readability or writability.
        ev_timer & ev_periodic: Manages relative and absolute timeouts, gracefully handling
               system time jumps.
        ev_signal & ev_child: Handles UNIX signals and tracks child process status changes.
        ev_stat: Tracks file system path changes using OS mechanics like Linux inotify.
        ev_async: Facilitates fast intra-thread communication between distinct event loops.
Fork Support:
    Fully handles process forks, automatically re-arming underlying kernel mechanisms that
    don't inherently survive forks. [1] (https://github.com/shadowsocks/libev)Highly
Embeddable:
    It contains no external dependencies, features a small codebase, and is structured to be
    dropped directly into other projects.

Video link for this example : https://youtu.be/QP-cXgRqcpQ
