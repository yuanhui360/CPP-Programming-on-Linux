#include <iostream>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/vtimes.h>
#include <unistd.h>
#include <ulimit.h>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <future>

int using_memory(int i) {
    char   myArray[8192];
    memset(myArray, '\0', sizeof(myArray)+1);
    for ( size_t j = 0; j<sizeof(myArray); j++) {
        myArray[j] = 'A';
    }
    return 0;
}

void read_write_files(char* inFileName, char* outFileName) {

    std::vector<std::string>    myVec;
    std::string line;
    int i;

    std::ifstream myFile(inFileName);
    if (myFile.is_open())
    {
        i =0;
        while ( getline(myFile, line))
        {
        myVec.push_back(line);
             i++;
            if ( i == 1 || i == 20000 )
                std::cout << line << std::endl;
        }
        std::cout << line << std::endl;
        myFile.close();
    }

    std::ofstream outFile(outFileName);
    if (outFile.is_open()) {

        for ( auto it=myVec.begin(); it!=myVec.end(); it++) {
            outFile << *it << std::endl;
        }
        outFile.flush();

        outFile.close();
    }

}

int main( int argc, char* argv[])
{
    struct rusage  *pUsg = new struct rusage();
    int ret;

    if ( argc != 3)
    {
         std::cout << " Usage : $0 <file-name> <output file>" << std::endl;
         return 1;
    }

    int totThread = 10;
    std::future<int> mTH[totThread];
    for (int i = 0; i<totThread; i++) {
        mTH[i] = std::async(using_memory, i);
    }

    for (int i = 0; i<totThread; i++) {
        mTH[i].get();
    }
 
    read_write_files(argv[1], argv[2]);
    //  ret = getrusage(RUSAGE_CHILDREN, pUsg);
    ret = getrusage(RUSAGE_SELF, pUsg);
    std::cout << "------ RUSAGE_SELF Process Usgae --------------" << std::endl;
    if ( ret == 0 ) {
        std::cout << pUsg->ru_isrss << " Unshared memory used for stack" << std::endl;
        std::cout << pUsg->ru_idrss << " Unshared memory used for data" << std::endl;
        std::cout << pUsg->ru_ixrss << " Shared memory with other processes" << std::endl;
        std::cout << pUsg->ru_maxrss << " Maximum of physical memory used" << std::endl;
        std::cout << pUsg->ru_majflt << " page faults by doing I/O (hard) " << std::endl;
        std::cout << pUsg->ru_minflt << " page faults without doing I/O (soft)" << std::endl;
        std::cout << pUsg->ru_msgrcv << " Number of IPC messages received" << std::endl;
        std::cout << pUsg->ru_msgsnd << " Number of IPC messages sent : " << std::endl;
        std::cout << pUsg->ru_nswap << " Swapped entirely out of main memory" << std::endl;
        std::cout << pUsg->ru_nvcsw << " voluntarily context switch" << std::endl;
        std::cout << pUsg->ru_nivcsw << " Involuntary context switch" << std::endl;
        std::cout << pUsg->ru_nsignals << " Number of signals received" << std::endl;
        std::cout << pUsg->ru_inblock  << " File system read from the disk" << std::endl;
        std::cout << pUsg->ru_oublock <<  " File system write to the disk" << std::endl;
        std::cout << pUsg->ru_utime.tv_sec << " User CPU time used" << std::endl;
        std::cout << pUsg->ru_stime.tv_sec << " System CPU time used" << std::endl;
    }
    else {
        std::cout << "Failed call getrusage() " << std::endl;
    }

    return ret;
}

