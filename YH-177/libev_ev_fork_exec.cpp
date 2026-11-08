#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>
#include <ev.h>            // libev header file

// Declare fork watcher and 2 timer watchers
ev_fork       fork_watcher;
ev_timer      parent_timer_watcher;          // Run in parent process
ev_timer      child_timer_watcher;           // Run in child process

int  parent_timeout_count;
int  child_timeout_count;

// ev_fork callback function of fork_watcher
static void fork_cb(struct ev_loop *loop, ev_fork *w, int revents)
{
        (void) loop;
        (void) w;
        (void) revents;

        // print log info that fork callback function has been invoked
        std::cout << "[Child  PID " << getpid() << "] ev_fork callback triggered from ev_loop_fork()" << std::endl;

        // Add perform application-specific child re-initializatione here
        std::cout << "[Child  PID " << getpid() << "] Stop parent_timer_watcher" << std::endl;
        ev_timer_stop(loop, &parent_timer_watcher);
}

// parent ev_timer timeout callback function
static void parent_timeout_cb(EV_P_ ev_timer *w, int revents)
{
        (void) w;
        (void) revents;

        parent_timeout_count++;
        std::cout << "[Parent PID " << getpid() << "] Timeout callback process ("
                      << parent_timeout_count << ")" << std::endl;

        if ( parent_timeout_count >= 5 ) {
                printf("[Parent PID %d] Timeout reached limit, Stop Parent Event Loop\n", getpid());
                ev_break(EV_A_ EVBREAK_ALL);                // EVBREAK_ONE or EVBREAK_ALL
        }
}


// child  ev_timer timeout callback function
static void child_timeout_cb(EV_P_ ev_timer *w, int revents)
{
        (void) w;
        (void) revents;

        child_timeout_count++;
        std::cout << "[Child  PID " << getpid() << "] Timeout callback process ("
                      << child_timeout_count << ")" << std::endl;

        if ( child_timeout_count >= 3 ) {
                printf("[Child  PID %d] Timeout reached limit, Stop Parent Event Loop\n", getpid());
                ev_break(EV_A_ EVBREAK_ALL);                // EVBREAK_ONE or EVBREAK_ALL
        }
}

int main(int argc, char *argv[]) {

        (void) argc;
        // Add 1. set parent_timeout_count and child_timeout_count to 0
        parent_timeout_count = 0;
        child_timeout_count = 0;

        if ( argc < 2 ) {
        printf("Usage   : %s <execute file>\n", argv[0]);
        printf("Example :\n");
        printf("        : %s my_hello_world\n", argv[0]);
        exit(EXIT_FAILURE);
        }

    std::cout << "[Parent PID " << getpid() << "] Program Starting ("
                      << argv[0] << ")" << std::endl;

        // Add 2. Get the default event loop
        struct ev_loop *loop = EV_DEFAULT;

        // Add 3. Initialize and Start ev_fork and parent ev_timer watcher
        ev_fork_init(&fork_watcher, fork_cb);
        ev_fork_start(loop, &fork_watcher);

        ev_timer_init(&parent_timer_watcher, parent_timeout_cb, 5.0, 5.0);
        ev_timer_start(loop, &parent_timer_watcher);

        // fork() system call split process into 2, parent and child
    pid_t pid = fork();
    if (pid < 0) {
        printf("[Parent PID %d] Fork System Call Failed.\n", getpid());
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // ----- Child Process -----
        printf("[Child  PID %d] Child Process - Created Successfully (%s)\n", getpid(), argv[1]);

                // Add 4. Notify the event loop that fork occurred
                //        In the child process you must immediately call ev_loop_fork() after a fork()
                ev_loop_fork(loop);

                // Add 5. Initialize and Start child ev_timer watcher
                ev_timer_init(&child_timer_watcher, child_timeout_cb, 5.0, 5.0);
                ev_timer_start(loop, &child_timer_watcher);

                // Add 6. Run the event loop in the child process
                ev_run(loop, 0);

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

                // Add 7. Run event loop and handle parent process
                ev_run(loop, 0);
    }

    printf("[Parent PID %d] Program Completed Successfully\n", getpid());
    exit(EXIT_SUCCESS);
}
