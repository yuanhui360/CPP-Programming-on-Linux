#include <iostream>
#include <fstream>
#include <chrono>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <future>
#include <mutex>

using namespace std;
#define MAX_THREADS    8
std::mutex mtx;

int ThreadReadFile(int t_id, char  *pFileName)
{
    char  inFileName[30];
    std::string line;
    int i, initPos;
    initPos = t_id * 2500 * 27;
    memset(inFileName, '\0', sizeof(inFileName) + 1);
    memcpy(inFileName, pFileName, strlen(pFileName));

    ifstream myFile(inFileName);
    if (myFile.is_open())
    {
        i =0;
        myFile.seekg(initPos, ios::beg);
        while ( getline(myFile, line) && i < 2500)
        {
            i++;
            mtx.lock();
            std::cout << line << " -> Thread : " << t_id << " : " << i << std::endl;
            mtx.unlock();
        }
        myFile.close();
    }
    else {
        std::cout << "can not open file example.txt for reading" << std::endl;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    int j, ret = 0;
    std::future<int> rc[MAX_THREADS];

    if ( argc != 2)
    {
        std::cout << " Usage : myThreadReadFile <file-name> " << std::endl;
        return 1;
    }

    try {
        for ( j=0; j<MAX_THREADS; j++) {
            rc[j] = std::async(ThreadReadFile, j, argv[1]);
        }

        for ( j=0; j<MAX_THREADS; j++) {
            ret = rc[j].get();
        }
    }
    catch ( std::exception &ec ) {
        std::cout << "[Exception] " << ec.what() << std::endl;
    }
    return ret;
}
