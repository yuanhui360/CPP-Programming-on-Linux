#include <iostream>
#include <pthread.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <unistd.h>
#include <mutex>
#include <queue>          // std::queue
#include <random>         // Required for modern random number generation
#include <ev.h>           // libev header file

using namespace std;
#define MAX_THREADS    5

static ev_async     async_watcher;
int    prc_pthread = 0;
int    tot_pthread = 0;

std::queue<std::string> data_queue;   // FIFO queue store shared data from multithread
std::mutex queue_mutex;               // Lockable object protect shared data concurrently
                                      // modified by multi thread
// parameter pass to thread function
struct threadParam
{
    int    t_id;
};

void async_process_data_cb(EV_P_ ev_async *w, int revents)
{
    (void) w;
    (void) revents;

    /*
     * time_t now = time(0);       // current system date/time
     * char*   dt = ctime(&now);   // convert time to string
     * std::cout << "INFO : Processing Data Queue started   --> " << dt;
     */

    while ( ! data_queue.empty() ) {
        std::cout << "INFO : [ASYNC CB] (" <<  data_queue.front();
        data_queue.pop();
    }

    // Add if processed thread >= tot thread then stop watcher and break
    if ( prc_pthread >= tot_pthread - 1 ) {
        ev_async_stop(loop, &async_watcher);
        ev_break(loop, EVBREAK_ALL);
        std::cout << "INFO : [ASYNC CB] (" << prc_pthread
                  << ") Event Loop Stop (EVBREAK_ALL) " << std::endl;
    }
}

int random_integer()
{
    // 1. Obtain a random seed from the hardware
    std::random_device rd;

    // 2. Initialize the Standard mersenne_twister_engine with the seed
    std::mt19937 gen(rd());

    // 3. Define the range (inclusive: from 1 to 10 in this example)
    std::uniform_int_distribution<int> distrib(1, 10);

    int random_num = distrib(gen);
    return random_num;
}

/*
 *     pthread_generate_data()
 *     {
 *         1. Print pthread start information
 *         2. sleep random seconds and push data to data_queue
 *         3. print pthread finish information
 *     }
 */

void * pthread_generate_data(void *tParam)
{
    struct threadParam  *mData = static_cast<threadParam*>(tParam);
    std::stringstream ss;

    time_t now = time(0);       // current system date/time
    char*   dt = ctime(&now);   // convert time to string
    std::cout << "INFO : [P_THREAD] (" << mData->t_id << ") Thread Started   --> " << dt;

    int rd = random_integer();
    struct timespec    ts;
    ts.tv_sec  = mData->t_id + rd;
    ts.tv_nsec = 500000000;          // 500,000,000 nanoseconds (0.5 seconds)
    nanosleep(&ts, nullptr);         // high-resolution sleep

    now = time(0);       // current system date/time
    dt  = ctime(&now);   // convert time to string
    ss << mData->t_id << ") Data Process ("
       << ts.tv_sec << ") --> " << dt;
    std::lock_guard<std::mutex> lock(queue_mutex);
    data_queue.push(ss.str());
    std::cout << "INFO : [P_THREAD] (" << mData->t_id << ") Pushed to Queue  --> " << dt;
    ev_async_send(EV_DEFAULT_ &async_watcher);

    sleep(rd);

    prc_pthread++;
    now = time(0);       // current system date/time
    dt  = ctime(&now);   // convert time to string
    std::cout << "INFO : [P_THREAD] (" << mData->t_id << ") Thread Finished  --> " << dt;

    pthread_exit(NULL);
}

/*
 *  This is multithread multiple-writer-single-reader queue example.
 */

int main(int argc, char* argv[])
{
    pthread_t  threads[MAX_THREADS];
    struct threadParam mTD[MAX_THREADS];

    if ( argc != 2)
    {
        std::cout << " Usage : " << argv[0] << " <num. pthread> " << std::endl;
        std::cout << " Note  : Maximum Pthread Limit is : " << MAX_THREADS << std::endl;
        return -1;
    }

    int num_pthread = std::atoi(argv[1]);
    if ( num_pthread > MAX_THREADS ) {
        std::cout << " pthread num " << num_pthread << " is over the Maximum Limit "
                  << MAX_THREADS << std::endl;
        return -1;
    }

    // Add 1. Create the default event loop
    // Add 2. Initialize the ev_async watcher and bind it to our callback
    // Add 3. Start the async watcher on the loop

    struct ev_loop   *loop = EV_DEFAULT;
    ev_async_init(&async_watcher, async_process_data_cb);
    ev_async_start(loop, &async_watcher);

    // Loop create pthread using pthread_create()
    for ( int j = 0; j < num_pthread; j++)
    {
        mTD[j].t_id = j;
        int rc = pthread_create(&threads[j], NULL, pthread_generate_data, (void*)&mTD[j]);
        if (rc){
           cout << "ERROR: unable to create thread(" << j << ") -- Error Code : " << rc << endl;
           exit(-1);
        }

        // Add 4. calculate tot_pthread created
        tot_pthread++;
    }

    // Add 5. Run Event Loop
    ev_run(loop, 0);

    // Wait for all threads to complete using pthread_join()
    // man pthread_join
    // int pthread_join(pthread_t thread, void **retval);

    for ( int j = 0; j < num_pthread; j++)
    {
        int rc = pthread_join(threads[j], nullptr);
        if (rc) {
           cout << "Error: unable to join thread(" << j << ") -- Error Code : " << rc << endl;
           exit(-1);
        }
    }

    std::cout << "INFO : [MAIN]     ALL " << num_pthread << " threads have completed.\n";

    // process and print out all data in data_queue
    // process_data();
    return 0;
}
