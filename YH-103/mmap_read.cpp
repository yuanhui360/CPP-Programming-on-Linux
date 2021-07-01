#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct stat fileStatus;

    if ( argc != 2)
    {
        std::cout << " Usage : mmap_read <file-name> " << std::endl;
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);           // Step 1 open file get fd
    fstat(fd, &fileStatus);                     // Step 2 get file size
    size_t fsize = fileStatus.st_size;          // <- Total size, in bytes
    std::cout << "File : " << argv[1] << " <-- size : " << fsize << std::endl;

    void *addr = mmap(NULL, fsize, PROT_READ, MAP_SHARED, fd, 0); // Step 3 mapping
    std::cout << (char*)addr << std::endl;

    munmap(addr, fsize);               // Step 4 unmapping
    close(fd);                         // Step 5 close()

    return EXIT_SUCCESS;
}
