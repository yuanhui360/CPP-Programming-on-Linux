#include <iostream>
#include <fstream>
#include <chrono>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <atomic>         // std::atomic, std::memory_order_relaxed

/*
 *  Atomic:
 *  Atomic operations modify data in a single clock tick, so that it is impossible
 *  for ANY other thread to access the data in the middle of such an update.
 *
 *  Additionally, atomic objects have the ability to synchronize access to other
 *  non-atomic objects in their threads by specifying different memory orders.
 *
 *  Memory order
 *  Used as an argument to functions that conduct atomic operations to specify
 *  how other operations on different threads are synchronized.
 *
 *  memory_order_relaxed,   // relaxed
 *  memory_order_consume,   // consume
 *  memory_order_acquire,   // acquire
 *  memory_order_release,   // release
 *  memory_order_acq_rel,   // acquire/release
 *  memory_order_seq_cst    // sequentially consistent
 *
 *  std::atomic::operator=
 *  std::atomic::operator++
 *  std::atomic::operator--   // uses sequential consistency (memory_order_seq_cst).
 *
 */

using namespace std;

std::atomic<long> prgTotal(0);

void * myCalculator(void* id)
{
    long subTot = 0;
    long tid;
    tid = (long)id;

    for ( int i = 0; i < 100; i++ ) {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        subTot += i;
        // prgTotal += i;
        prgTotal.fetch_add(i, std::memory_order_relaxed);
    }
    std::cout << "Thread " << tid << " Sub-Total : " << subTot << std::endl;
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    if ( argc != 2)
    {
        std::cout << " Usage : myThreadCalculate <number threads> " << std::endl;
        return 1;
    }

    int numThread = std::stoi(argv[1]);
    pthread_t  threads[numThread];
    pthread_attr_t attr;
    long j;
    int rc;
    void *status;

    prgTotal.store(10, std::memory_order_relaxed);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for ( j=0; j<numThread; j++) {
        rc = pthread_create(&threads[j], NULL, myCalculator, (void*)j);
        if (rc){
           cout << "Error:unable to create thread," << rc << endl;
           exit(-1);
        }
    }

    pthread_attr_destroy(&attr);
    for ( j=0; j<numThread; j++) {
        rc = pthread_join(threads[j], &status);
        if (rc){
           cout << "Error:unable to thread join " << rc << endl;
           exit(-1);
        }
    }

    long myTot = prgTotal;
    std::cout << "Program Total : " << myTot << std::endl;
    pthread_exit(NULL);
}
