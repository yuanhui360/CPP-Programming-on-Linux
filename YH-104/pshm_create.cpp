#include <iostream>
#include <ctype.h>
#include <string.h>
#include "pshm.h"

/*
 *
 *  1) Create shared memory object
 *     int fd = shm_open(const char *name, int oflag, mode_t mode);
 *  2) set its size to the size of our structure.
 *     int ftruncate(int fd, off_t length);
 *  3) Map the object into the caller's address space.
 *     void *addr = mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
 *     struct shmbuf *shmp = static_cast<struct shmbuf*>(addr);
 *  4) Initialize semaphores as process-shared, with value 0.
 *     sem_init(&shmp->sem1, 1, 0);
 *     sem_init(&shmp->sem2, 1, 0);
 *
 */

int main(int argc, char *argv[])
{
    char opt[10];
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));

    if (argc != 2) {
        std::cerr << "Usage : pshm_create <Name>" << std::endl;
        exit(EXIT_FAILURE);
    }

    char *shmpath = argv[1];

    int fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        std::cerr << "Failed Open POSIX shared memory objects" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, sizeof(struct shmbuf)) == -1) {
        std::cerr << "Failed of setting shared memory objects size" << std::endl;
        exit(EXIT_FAILURE);
    }

    /*
     * Map the object into the caller's address space.
     */

    void *addr = mmap(NULL, sizeof(struct shmbuf), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    struct shmbuf *shmp = static_cast<struct shmbuf*>(addr);

    /*
     * Initialize semaphores as process-shared, with value 0.
     */

    if (sem_init(&shmp->sem1, 1, 0) == -1) { errExit("sem_init-sem1"); }
    if (sem_init(&shmp->sem2, 1, 0) == -1) { errExit("sem_init-sem2"); }
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

    /*
     * Unlink the shared memory object. Even if the peer process
     * is still using the object, this is okay. The object will
     * be removed only after all open references are closed.
     */

    shm_unlink(shmpath);

    exit(EXIT_SUCCESS);
}
