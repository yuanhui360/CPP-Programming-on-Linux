#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "shared_memory_object.hpp"
#include <iostream>
#include <cstdio>

using namespace boost::interprocess;

/*
 *  class shared_memory_object
 *  A class that wraps a shared memory mapping that can be used to
 *  create mapped regions from the mapped files
 */

int main ()
{
   char opt[10];
   char buffer[1024];
   memset(buffer, '\0', sizeof(buffer));
   try{

      /*
       *  1) Remove shared memory object on construction and destruction
       *  2) Create a shared memory object.
       *  3) Set Size
       *  4) Map the whole shared memory in this process
       *  5) Get the address of the mapped region (pointer)
       *  6) initialze scoped_lock<interprocess_mutex>
       */

      struct shm_remove
      {
         shm_remove() { shared_memory_object::remove("MySharedMemory"); }
         ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
      } remover;

      shared_memory_object shm(create_only, "MySharedMemory",read_write);
      shm.truncate(sizeof(shared_memory_message));    // Set size
      mapped_region region(shm, read_write);
      void *addr = region.get_address();

      //Construct the shared structure in memory
      shared_memory_message * data = new (addr) shared_memory_message;
      scoped_lock<interprocess_mutex> lock(data->mutex);
      if ( lock.owns() ) {
          memcpy(data->items, "Initial Create Message", strlen("Initial Create Message"));
          lock.unlock();
      }

      for (;;) {
          std::cout << "Select Option (lock/read/write/unlock/exit) : ";
          memset(opt, '\0', sizeof(opt));
          std::cin.getline(opt, sizeof(opt));
          if ( !strcmp(opt, "lock") ) {
              if ( !lock.owns()) {
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
