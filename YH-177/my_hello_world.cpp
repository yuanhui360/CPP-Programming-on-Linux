#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[])
{
        (void) argc;
        (void) argv;

        std::cout << "[Exec() PID " << getpid() << "] Hello Wolrd From Hui ("
                      << argv[0] << ")" << std::endl;
        return 0;
}
