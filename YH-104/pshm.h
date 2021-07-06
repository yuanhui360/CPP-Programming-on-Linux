#ifndef PSHM_H
#define PSHM_H

#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define BUF_SIZE 1024

/*
 *  Define a structure that will be imposed on the shared memory object
 *  size_t cnt;             Number of bytes used in 'buf'
 */

struct shmbuf {
    sem_t  sem1;            /* POSIX unnamed semaphore */
    sem_t  sem2;            /* POSIX unnamed semaphore */
    char   buf[BUF_SIZE];   /* Data being transferred */
};

void show_sem_value(struct shmbuf *shmBuff)
{
    int val_1, val_2;
    ::sem_getvalue(&shmBuff->sem1, &val_1);
    ::sem_getvalue(&shmBuff->sem2, &val_2);
    std::cout << "shmp->sem1 : " <<  val_1 << " shmp->sem2: " << val_2 << std::endl;
}

#endif
