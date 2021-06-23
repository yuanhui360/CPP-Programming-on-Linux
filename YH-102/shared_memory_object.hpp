#ifndef MY_ANONYMOUS_MUTEX
#define MY_ANONYMOUS_MUTEX

#include <boost/interprocess/sync/interprocess_mutex.hpp>

struct shared_memory_message
{

   // Mutex to protect access to the queue
   boost::interprocess::interprocess_mutex mutex;

   // Items to fill
   // void *buffer = malloc (1024);
   char   items[1024];
};

#endif
