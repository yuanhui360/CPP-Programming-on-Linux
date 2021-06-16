#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <string>
#include <cstdlib> //std::system
#include <iostream>

using namespace boost::interprocess;

/*
 *  To make two processes share vector data using shared memory
 *
 *  Define an STL compatible allocator of int type
 *  that allocates from the managed_shared_memory.
 *  This allocator will allow placing containers in the segment
 *
 *  Define a vector that uses the allocator to allocates its values from the segment
 *
 *  std::vector<int> myvector;
 */

typedef allocator<int, managed_shared_memory::segment_manager>  myAllocator;
typedef vector<int, myAllocator> MyVectorType;

int main(int argc, char *argv[])
{
      int vecStart = 0;
      int vecSize;
      char opt[10];
      memset(opt, '\0', sizeof(opt));
  
      /*
       *  Remove shared memory on construction and destruction
       *  Create a new segment with given name and size
       */

      struct shm_remove
      {
         shm_remove() { shared_memory_object::remove("MySharedMemory"); }
         ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
      } remover;

      managed_shared_memory segment(create_only, "MySharedMemory", 65536);

      /*
       *  Initialize shared memory STL-compatible allocator
       *  Construct a vector named "MyVector" in shared memory with argument alloc_inst
       */

      const myAllocator myAlloc (segment.get_segment_manager());
      MyVectorType *myvector = segment.construct<MyVectorType>("MyVector")(myAlloc);

      for(int i = 0; i < 10; ++i)  {
          myvector->push_back(i);
      }

      for ( auto &&myVecElem : *myvector ) {
          std::cout << myVecElem << ", ";
      }
      std::cout << std::endl;

      for (;;) {
          std::cout << "Select Option (add-10/read/push/pop/exit) : ";
          memset(opt, '\0', sizeof(opt));
          std::cin.getline(opt, sizeof(opt));
          if ( !strcmp(opt, "add-10") ) {
              vecStart = myvector->front() + 10;
              myvector->clear();
              for(int i = 0; i < 10; ++i)  //Insert data in the vector
                  myvector->push_back( i + vecStart );
          }
          else if ( !strcmp(opt, "read") ) {
              vecSize = myvector->size();
              for ( int i =0; i < vecSize; i++ ) {
                  std::cout << myvector->at(i) << ", ";
              }
              std::cout << std::endl;
          }
          else if ( !strcmp(opt, "push") ) {
              int myVal = myvector->back();
              myvector->push_back(myVal + 1);
          }
          else if ( !strcmp(opt, "pop") ) {
              myvector->pop_back();
          }
          else {
              break;
          }
      }

   //When done, destroy the vector from the segment
   segment.destroy<MyVectorType>("MyVector");
   return 0;
}
