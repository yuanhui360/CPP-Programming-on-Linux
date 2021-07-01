#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PERM 0644

int main(int argc, char *argv[])
{

    struct stat fileStatus;

    if ( argc != 3)
    {
        std::cout << " Usage : mmap_update <file-name> <Message>" << std::endl;
        return 1;
    }

    size_t length = strlen(argv[2]) + 1;
    int fd = open(argv[1],  O_RDWR, PERM);      // Step 1 open file get fd
    fstat(fd, &fileStatus);                     // Step 2 get file size
    size_t fsize = fileStatus.st_size;
    std::cout << "File : " << argv[1] << " <-- size : " << length << ":" << fsize << std::endl;

    void  *addr = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // Step 3 mapping
    std::cout << "Read  : " << (char*)addr;

    memcpy(addr, argv[2], strlen(argv[2]));
    std::cout << "After : " << (char*)addr;

    msync(addr, fsize, MS_SYNC);       // Step 4 Sync memory to file
    munmap(addr, fsize);               // Step 5 unmapping
    close(fd);                         // Step 6 close

    return EXIT_SUCCESS;
}
