Signals are a limited form of inter-process communication (IPC), typically used in Unix, Unix-like, and other POSIX-compliant operating systems. 
A signal is an asynchronous notification sent to a process or to a specific thread within the same process to notify it of an event. Signals 
originated in 1970s Bell Labs Unix and were later specified in the POSIX standard.

When a signal is sent, the operating system interrupts the target process' normal flow of execution to deliver the signal. Execution can be 
interrupted during any non-atomic instruction. If the process has previously registered a signal handler, that routine is executed. Otherwise, 
the default signal handler is executed.

This example in this video demonstrated how to program using Linux signal to communicate in same process, among threads and between processes.

The video of this example link : https://youtu.be/LLd8haPo1q4
