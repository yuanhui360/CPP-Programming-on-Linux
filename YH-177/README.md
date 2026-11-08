This example covers libev, a high-performance event loop library. As we know, one of libev's standout features is its diverse 
set of specialized event types watcher. In previous example, we have covered basic concept of ev_stat, ev_timer, ev_signal, and ev_async watchers. 
In particular,ev_async serves as a powerful tool designed specifically for inter-thread synchronization and communication.

In this example, it'll discuss another unique watcher type in libev: the ev_fork watcher, specially designed for performing fork inter-process 
event loop.

Here is Architectural Comparison of Multithreading vs. Multi-Process.
Multithreading and Multi-Process Programming in C++ are 2 popular execution models to maximize modern CPU utilization, both of them are used to 
split workloads and run tasks concurrently to improve overall application performance and efficiency:

 - Multithreading : tasks run within the same process and sharing the exact same memory space.  (heap, data segments and file descriptors)
 - Multi-process  : Tasks runs in separate processes with distinct Process IDs (PIDs)  and entirely separated memory spaces.

Additionally, multi-process programming is not only just boost execution efficiency, it also provides strong fault isolation of application. 
For example, if one child process crashes, the rest of the application can keep running safely.

Here is high level waorking flow for ev_fork.
1) Initilize event loop and start parent watcher in main process.
2) When a process calls fork(), the operating system duplicates the parent process's entire  memory space and environment to create a child process.
3) In inside child process, some parent environment variable need to re-initialize to prevent  unexpected behave triggered inside child process.
4) The ev_fork watcher detects when a fork has occurred and triggers a dedicated callback to re-arm or reinitialize the event loop environment in
   the child process.
5) When a child process terminates, the parent process resumes event loop and continues running.
