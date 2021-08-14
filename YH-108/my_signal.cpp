#include <iostream>
#include <csignal> // This file is part of the GNU ISO C++ Library.
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/*
 * NAME
 *        raise - send a signal to the caller
 *
 * SYNOPSIS
 *        #include <signal.h>
 *        int raise(int sig);
 *
 * DESCRIPTION
 *        The raise() function sends a signal to the calling process or thread.
 */

using namespace std;

void signalHandler( int signum )
{
    cout << "raise signal (" << signum << ") ";

    switch (signum) {
        case 1:
            std::cout << "SIGHUP";
            break;
        case 2:
            std::cout << "SIGINT";
            break;
        case 2:
            std::cout << "SIGINT";
            break;
        case 10:
            std::cout << "SIGUSR1";
            break;
        case 12:
            std::cout << "SIGUSR2";
            break;
        default:
            exit(signum);
            break;
    }

    cout << " received." << std::endl;;
}

int main (int argc, char* argv[])
{
    int i = 0;
    char opt[10];

    // register signal SIGINT and signal handler
    signal(SIGHUP, signalHandler);   // 1
    signal(SIGINT, signalHandler);   // 2
    signal(SIGUSR1, signalHandler);   // 10
    signal(SIGUSR2, signalHandler);   // 12

    pid_t myPID = getpid();
    std::cout << "My Process : " << myPID << std::endl;
    i = 1;

    while(i){
        memset(opt, '\0', sizeof(opt));
        std::cout << "Waiting input signal (1,2,10,12) : ";
        std::cin.getline(opt, sizeof(opt));
        i = std::atoi(opt);
        if (i)
            raise(i);
    }

    return 0;
}
