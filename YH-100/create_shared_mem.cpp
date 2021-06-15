#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>
#include <cstdlib> //std::system
#include <sstream>

using namespace boost::interprocess;

int main (int argc, char *argv[])
{

    char buffer[1024];
    char opt[10];

    memset(buffer, '\0', sizeof(buffer));
    memset(opt, '\0', sizeof(opt));

    struct shm_remove
    {
       shm_remove() {  shared_memory_object::remove("MySharedMemory"); }
       ~shm_remove(){  shared_memory_object::remove("MySharedMemory"); }
    } remover;

    managed_shared_memory segment(create_only, "MySharedMemory", 65536);
    managed_shared_memory::size_type free_memory_before = segment.get_free_memory();
    void * shptr = segment.allocate(1024);
    managed_shared_memory::size_type free_memory_after = segment.get_free_memory();

    std::cout << "Free Memeory Before : " << free_memory_before << std::endl;
    std::cout << "Free Memeory After  : " << free_memory_after << std::endl;
    std::cout << "Free Memeory Diff.  : " << free_memory_before - free_memory_after << std::endl;

    /*
     *  An handle from the base address can identify any byte of the shared
     *  memory segment even if it is mapped in different base addresses
     */
    managed_shared_memory::handle_t handle = segment.get_handle_from_address(shptr);
    std::cout << "Shared Memory handle : " << handle << std::endl;

    for (;;) {
        std::cout << "Select Option (write/read/exit) : ";
        memset(opt, '\0', sizeof(opt));
        std::cin.getline(opt, sizeof(opt));
        if ( !strcmp(opt, "write") ) {
            std::cout << "-> Shared Memory : ";
            memset(buffer, '\0', sizeof(buffer));
            std::cin.getline(buffer, sizeof(buffer));
            memset(shptr, '\0', 1024);
            memcpy((char*)shptr, buffer, strlen(buffer));
        }
        else if ( !strcmp(opt, "read") ) {
            std::cout << "<- Share Memory : ";
            std::cout << (char*)shptr << std::endl;
        }
        else {
            break;
        }
    }

    return 0;
}
