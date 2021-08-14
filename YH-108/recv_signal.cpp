#include <iostream>
#include <csignal> // This file is part of the GNU ISO C++ Library.
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

using namespace std;

void signalHandler( int signum )
{
    cout << "Process received  signal (" << signum << ") ";

    switch (signum) {
        case SIGHUP:
            std::cout << "SIGHUP";
            break;
        case SIGINT:
            std::cout << "SIGINT";
            break;
        case SIGUSR1:
            std::cout << "SIGUSR1";
            break;
        case SIGUSR2:
            std::cout << "SIGUSR2";
            break;
        default:
            exit(signum);
            break;
    }
    cout << std::endl;
}

int main (int argc, char* argv[])
{
    int i = 0;

    // register signal SIGINT and signal handler
    signal(SIGHUP, signalHandler);   // 1
    signal(SIGINT, signalHandler);   // 2
    signal(SIGUSR1, signalHandler);   // 10
    signal(SIGUSR2, signalHandler);   // 12

    pid_t myPID = getpid();
    std::cout << "My Process : " << myPID << std::endl;
    i = 1;

    std::cout << "Pausing (waiting) signal .... " << std::endl;
    while(i){
        pause();
    }

    return 0;
}
