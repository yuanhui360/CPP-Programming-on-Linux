#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <csignal> // This file is part of the GNU ISO C++ Library.
#include <atomic>

using namespace std;

/*
 *  https://man7.org/linux/man-pages/man7/signal.7.html
 *  https://www-uxsup.csx.cam.ac.uk/courses/moved.Building/signals.pdf
 *
 *  10 SIGUSR1 Left for the programmers to do whatever they want.
 *  12 SIGUSR2 Left for the programmers to do whatever they want.
 *
 *  int signaled = 0;
 *
 *  std::sig_atomic_t
 *  An integer type which can be accessed as an atomic entity even in the presence of
 *  asynchronous interrupts made by signals.
 */

std::atomic<std::sig_atomic_t> signaled(0);

void signalHandler( int signum )
{
    signaled.store(signum, std::memory_order_relaxed);
}

void * threadFunction(void *t)
{
    long tid;
    tid = (long)t;

    std::cout << "Thread with id : " << tid << " is waiting signal ... " << std::endl;
    pause();  // waiting signal

    int sigNum = signaled;
    std::cout << "Thread with id : " << tid << " signal (" << sigNum << ") ";
    switch (sigNum) {
        case SIGUSR1:
            std::cout << "SIGUSR1";
            break;
        case SIGUSR2:
            std::cout << "SIGUSR2";
            break;
        default:
            exit(sigNum);
            break;
    }
    std::cout << " received." << std::endl;;
    pthread_exit(NULL);
}

int main (int argc, char* argv[])
{
     int rc;
     long tid;

     /*
      * Sends a signal to a specified POSIX thread in the same process as the caller.
      */

     if ( argc != 2)
     {
         std::cout << " Usage : pthread_signal <number threads> " << std::endl;
         return 1;
     }

     int numThread = std::stoi(argv[1]);
     pthread_t  threads[numThread];
     pthread_attr_t attr;
     void *status;

     // register signal SIGINT and signal handler
     signal(SIGUSR1, signalHandler);   // 10
     signal(SIGUSR2, signalHandler);   // 12

     // Initialize and set thread joinable
     pthread_attr_init(&attr);
     pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

     for ( tid=0; tid<numThread; tid++) {
         std::cout << "main() : creating thread, " << tid << std::endl;
         rc = pthread_create(&threads[tid], NULL, threadFunction, (void *)tid );
         if (rc){
             std::cout << "Error:unable to create thread," << rc << std::endl;
             exit(-1);
         }
     }

     sleep(3);

     /*
      *  pthread_kill() - Sends a signal to a specified POSIX thread in the same
      *                   process as the caller.
      *  int pthread_kill(pthread_t thread, int sig);
      */

     for ( tid=0; tid<numThread; tid++) {
         if ( tid % 2 == 0) {
             std::cout << "main() : sending SIGUSR1 (10) signal to  thread, " << tid << std::endl;
             pthread_kill( threads[tid], SIGUSR1);
         }
         else {
             std::cout << "main() : sending SIGUSR2 (12) signal to  thread, " << tid << std::endl;
             pthread_kill( threads[tid], SIGUSR2);
         }
     }

     // free attribute and wait for the other threads
     pthread_attr_destroy(&attr);
     for ( tid=0; tid<numThread; tid++) {
         rc = pthread_join(threads[tid], &status);
         if (rc){
             std::cout << "Error:unable to join," << rc << std::endl;
             exit(-1);
         }
     }

     std::cout << "Main: program exiting." << std::endl;
     pthread_exit(NULL);
}
