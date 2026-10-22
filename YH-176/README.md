This example discuss how to perform inter-thread communication using libev's ev_async watcher.  As we know, there are two commonly used techniques for 
high-performance programming: multithreading and event loops. The concept of multithreading is to split a massive process into multiple tasks and run them 
in parallel, especially in multi-CPU environments. These split thread tasks can be distributed across different CPUs to speed up the overall process. However, 
in the real world, we sometimes need to handle unpredictable situations. Using multithreading for these scenarios can consume a lot of system resources just 
waiting around. As we mentioned earlier, the concept of event loop or event notification is consume minimal resources to monitoring unpredictable event and 
process when it happened. so in general, it is better for handling unpredictable situations efficiently. Standard libev event loops (ev_loop) are traditionally 
single-threaded; they block or poll for events within a specific thread.  In a multithreading situation, if a background thread wants to tell the event loop 
thread to do something, simply calling standard libev functions across threads is unsafe because libev event loops are not thread-safe.
To resolve this issue, libev introduced a special type of watcher: the ev_async watcher.

Video link of this example : https://youtu.be/m75kvCCI2Wg
