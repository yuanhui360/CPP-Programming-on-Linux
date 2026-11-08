#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>

int main(int argc, char *argv[]) {

        (void) argc;
        if ( argc < 2 ) {
        printf("Usage   : %s <execute file>\n", argv[0]);
        printf("Example :\n");
        printf("        : %s my_hello_world\n", argv[0]);
        exit(EXIT_FAILURE);
        }

    std::cout << "[Parent PID " << getpid() << "] Program Starting ("
                      << argv[0] << ")" << std::endl;

        // fork() system call split process into 2, parent and child
    pid_t pid = fork();
    if (pid < 0) {
        printf("[Parent PID %d] Fork System Call Failed.\n", getpid());
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // ----- Child Process -----
        printf("[Child  PID %d] Child Process - Created Successfully (%s)\n", getpid(), argv[1]);

                std::stringstream ss;
                ss << "./" << argv[1];
        execl(ss.str().c_str(), argv[1], NULL);     // <-- exec() system call

        exit(EXIT_SUCCESS);
    }
    else {
        // ----- Parent Process Continue -----
        std::cout << "[Parent PID " << getpid() << "] Split Success - Parent Process Continuing" << std::endl;

        pid_t c_pid = wait(NULL); // Wait for the child to finish executing
        printf("[Parent PID %d] Child Process (%d) has finished executing.\n", getpid(), c_pid);

    }

    printf("[Parent PID %d] Program Completed Successfully\n", getpid());
    exit(EXIT_SUCCESS);
}
