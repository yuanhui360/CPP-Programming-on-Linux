Boost C++ Library Interprocess Mechanism is designed for Sharing information between processes.
This example using shared memory to communicate between 2 processes.

Processes And Threads
Boost.Interprocess does not work only with processes but also with threads.
Boost.Interprocess synchronization mechanisms can synchronize threads from
different processes, but also threads from the same process.

This is the case of classical shared memory or memory mapped files.
Once the processes set up the memory region, the processes can read/write the data
like any other memory segment without calling the operating system's kernel.
This also requires some kind of manual synchronization between processes.
