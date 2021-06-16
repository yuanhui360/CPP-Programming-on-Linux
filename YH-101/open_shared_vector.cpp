#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <string>
#include <cstdlib> //std::system
#include <iostream>

using namespace boost::interprocess;

/*
 *  Define an STL compatible allocator of ints that allocates from the managed_shared_memory.
 *  This allocator will allow placing containers in the segment
 *
 *  Alias a vector that uses the previous STL-like allocator so that allocates
 *  its values from the segment
 */

typedef allocator<int, managed_shared_memory::segment_manager>  myAllocator;
typedef vector<int, myAllocator> MyVectorType;

int main(int argc, char *argv[])
{
      int vecStartVal = 0;
      int vecSize;
      char opt[10];

      /*
       *  Open the managed segment
       *  Find the vector using the c-string name
       */

      managed_shared_memory segment(open_only, "MySharedMemory");
      MyVectorType *myvector = segment.find<MyVectorType>("MyVector").first;

      for ( auto &&myVecElem : *myvector ) {
          std::cout << myVecElem << ", ";
      }
      std::cout << std::endl;

      for (;;) {
          std::cout << "Select Option (add-10/read/push/pop/exit) : ";
          memset(opt, '\0', sizeof(opt));
          std::cin.getline(opt, sizeof(opt));
          if ( !strcmp(opt, "add-10") ) {
              vecStartVal = myvector->front() + 10;
              myvector->clear();
              for(int i = 0; i < 10; ++i)  //Insert data in the vector
                  myvector->push_back( i + vecStartVal );
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

   return 0;
}
