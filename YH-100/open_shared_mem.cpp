#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstdlib> //std::system
#include <sstream>
#include <iostream>
#include <stdio.h>

using namespace boost::interprocess;

int main (int argc, char *argv[])
{
    if ( argc != 2 ) {
        std::cerr << "Usage:  " << argv[0] << "handle" << std::endl;
        return -1;
    }

    char buffer[1024];
    char  opt[10];
    memset(buffer, '\0', sizeof(buffer));
    memset(opt, '\0', sizeof(opt));

    managed_shared_memory segment(open_only, "MySharedMemory");
    managed_shared_memory::handle_t handle = 0;

    //Obtain handle value
    std::stringstream s;
    s << argv[1];
    s >> handle;

    //Get buffer local address from handle
    void *msg = segment.get_address_from_handle(handle);
    for (;;) {

        std::cout << "Select Option (write/read/exit) : ";
        std::cin.getline(opt, sizeof(opt));

        if ( !strcmp(opt, "write") ) {
            std::cout << "-> Shared Memory : ";
            memset(buffer, '\0', sizeof(buffer));
            std::cin.getline(buffer, sizeof(buffer));
            memset(msg, '\0', 1024);
            memcpy((char*)msg, buffer, strlen(buffer));
        }
        else if ( !strcmp(opt, "read") ) {
            std::cout << "<- Share Memory : ";
            std::cout << (char*)msg << std::endl;
        }
        else {
            break;
        }
   }

   //Deallocate previously allocated memory
   segment.deallocate(msg);
   return 0;
}
