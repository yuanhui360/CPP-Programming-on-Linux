#include <string.h>
#include "pshm.h"

int main(int argc, char *argv[])
{
    char opt[10];
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));

    if (argc != 2) {
        std::cerr << "Usage : pshm_open <Name>" << std::endl;
        exit(EXIT_FAILURE);
    }

    char *shmpath = argv[1];

    /*
     * Open the existing shared memory object and map it
     * into the caller's address space.
     */

    int fd = shm_open(shmpath, O_RDWR, 0);
    if (fd == -1) { errExit("shm_open"); }

    struct shmbuf *shmp = static_cast<struct shmbuf*>(mmap(NULL, sizeof(*shmp),
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED, fd, 0));
    if (shmp == MAP_FAILED) { errExit("mmap"); }
    show_sem_value(shmp);

    for (;;) {
        std::cout << "Select Option (get/wait1/wait2/post1/post2/read/write/exit) : ";
        memset(opt, '\0', sizeof(opt));
        std::cin.getline(opt, sizeof(opt));
        if ( !strcmp(opt, "wait1") ) {
            if (sem_wait(&shmp->sem1) == -1) { errExit("sem_wait1"); }
            show_sem_value(shmp);
        }
        else if ( !strcmp(opt, "wait2") ) {
            if (sem_wait(&shmp->sem2) == -1) { errExit("sem_wait12"); }
            show_sem_value(shmp);
        }
        else if ( !strcmp(opt, "post1") ) {
            if (sem_post(&shmp->sem1) == -1) { errExit("sem_post1");}
            show_sem_value(shmp);
        }
        else if ( !strcmp(opt, "post2") ) {
            if (sem_post(&shmp->sem2) == -1) { errExit("sem_post2");}
            show_sem_value(shmp);
        }
        else if ( !strcmp(opt, "read") ) {
            std::cout << shmp->buf << std::endl;
        }
        else if ( !strcmp(opt, "get") ) {
            show_sem_value(shmp);
        }
        else if ( !strcmp(opt, "write") ) {
            std::cout << "-> Shared Memory : ";
            memset(buffer, '\0', sizeof(buffer));
            std::cin.getline(buffer, sizeof(buffer));
            memset(shmp->buf, '\0', 1024);
            memcpy(shmp->buf, buffer, strlen(buffer));
        }
        else {
            break;
        }
    }

    exit(EXIT_SUCCESS);
}
