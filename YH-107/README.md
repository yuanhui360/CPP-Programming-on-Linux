The atomic library provides components for fine-grained atomic operations allowing for lockless concurrent programming. 
Each atomic operation is indivisible with regards to any other atomic operation that involves the same object. 
Atomic objects are free of data races.

Atomic types are types that encapsulate a value whose access is guaranteed to not cause data races and can be used to 
synchronize memory accesses among different threads. Atomic operations modify data in a single clock tick, so that it is 
impossible for ANY other thread to access the data in the middle of such an update.

The video demon of this example link : https://youtu.be/Wy8zl9KtzZU
