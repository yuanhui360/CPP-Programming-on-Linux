#include <iostream>
#include <csignal> // This file is part of the GNU ISO C++ Library.
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

using namespace std;

int main (int argc, char* argv[])
{
    int mySIG = 0;
    char opt[10];
    pid_t myPID;

    // register signal SIGINT and signal handler
    if (argc != 2)
    {
        std::cout << "Usage: send_signal  <process ID> " << std::endl;
        return 1;
    }

    myPID = atoi(argv[1]);
    mySIG = 1;
    while(mySIG){
        memset(opt, '\0', sizeof(opt));
        std::cout << "Waiting input signal (1,2,10,12) : ";
        std::cin.getline(opt, sizeof(opt));
        mySIG = std::atoi(opt);

        /*
         *  kill - send signal to a process
         *
         *  The kill() system call can be used to send any signal to any
         *  process group or process
         *
         *  int kill(pid_t pid, int sig);
         */

        if (mySIG)
            kill(myPID, mySIG);
    }

    return 0;
}
