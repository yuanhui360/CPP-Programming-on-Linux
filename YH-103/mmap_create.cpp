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

    if ( argc != 3)
    {
        std::cout << " Usage : mmap_create <file-name> <Message>" << std::endl;
        return 1;
    }

    size_t fsize = strlen(argv[2]) + 1;
    int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, PERM);   // Step 1 open file get fd
    std::cout << "File : " << argv[1] << " <-- size : " << fsize << std::endl;

    lseek(fd, fsize-1, SEEK_SET);     // Step 2 set file size
    write(fd, "\n", 1);

    void  *addr = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // Step 3 mapping
    memcpy(addr, argv[2], strlen(argv[2]));
    msync(addr, fsize, MS_SYNC);       // Step 4 Sync memory to file
    munmap(addr, fsize);               // Step 5 unmapping
    close(fd);                         // Step 6 close file

    return EXIT_SUCCESS;
}
