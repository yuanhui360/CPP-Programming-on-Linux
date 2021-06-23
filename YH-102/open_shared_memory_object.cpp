#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "shared_memory_object.hpp"
#include <iostream>
#include <cstdio>

using namespace boost::interprocess;

int main ()
{
    char opt[10];
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));

    /*
     *  1) Open the shared memory object.
     *  2) Map the whole shared memory in this process
     *  3) Get the address of the mapped region
     *  4) Construct the shared structure in memory
     *  5) initialze scoped_lock<interprocess_mutex>
     */

    try {

        shared_memory_object shm(open_only, "MySharedMemory", read_write);
        mapped_region region(shm, read_write);
        void *addr = region.get_address();
        shared_memory_message *data = static_cast<shared_memory_message*>(addr);
        scoped_lock<interprocess_mutex> lock(data->mutex);

        for (;;) {
            std::cout << "Select Option (lock/read/write/unlock/exit) : ";
            memset(opt, '\0', sizeof(opt));
            std::cin.getline(opt, sizeof(opt));
            if ( !strcmp(opt, "lock") ) {
                if ( !lock.owns() ) {
                    lock.lock();
                }
            }
            else if ( !strcmp(opt, "read") ) {
                std::cout << data->items << std::endl;
            }
            else if ( !strcmp(opt, "write") ) {
                if ( lock.owns() ) {
                    std::cout << "-> Shared Memory : ";
                    memset(buffer, '\0', sizeof(buffer));
                    std::cin.getline(buffer, sizeof(buffer));
                    memset(data->items, '\0', 1024);
                    memcpy(data->items, buffer, strlen(buffer));
                }
                else {
                    std::cout << "Other process locked shared_memory_object" << std::endl;
                }
            }
            else if ( !strcmp(opt, "unlock") ) {
                lock.unlock();
            }
            else {
                break;
            }
        }
    }
    catch(interprocess_exception &ex){
       std::cout << ex.what() << std::endl;
       return 1;
    }
   return 0;
}
