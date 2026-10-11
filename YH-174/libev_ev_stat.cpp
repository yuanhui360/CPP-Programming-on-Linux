#include <stdio.h>
#include <ev.h>         // libev header file
#include <sys/stat.h>
#include <ctime>

// Allocate/declare the stat watcher and timeout watcher structure
ev_stat  file_watcher;           // <-- for monitoring file stat change
ev_timer timeout_watcher;        // <-- for timeout has been reached
int timeout_count = 0;

// timeout callback function
static void timeout_cb(EV_P_ ev_timer *w, int revents)
{
    timeout_count++;
    printf("INFO : Timeout callback at <%lld - %lld> -- (%d - %d) ... \n",
            (long long) w->at, (long long) w->repeat,
            revents, timeout_count);
    if ( timeout_count >= 5 ) {
        printf("INFO : Stop event loop callbacl (%d) ... \n", revents);
        ev_break(EV_A_ EVBREAK_ONE);             // EVBREAK_ONE or EVBREAK_ALL
    }
}


// callback function triggers whenever the file attributes changed
static void file_changed_cb(EV_P_ ev_stat *w, int revents) {

    char time_str[64];                   // Date Format: YYYY-MM-DD HH:MM:SS
    memset(time_str, '\0', 64);
    struct tm tm_buf;

    printf("INFO : File <%s> attribute changed callback (%d) !\n", w->path, revents);

    if ( w->attr.st_nlink == 0 ) {
        printf("ERROR : File <%s> not exist or unreachable. \n", w->path);
    } else {
        if ( w->prev.st_size > 0 ) {
            localtime_r(&w->prev.st_mtime, &tm_buf);
            std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &tm_buf);
            printf("INFO : Old size: %lld bytes, modified time : %s \n",
                    (long long) w->prev.st_size, time_str);
        }
        if ( w->attr.st_size > 0 ) {
            localtime_r(&w->attr.st_mtime, &tm_buf);
            std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &tm_buf);
            printf("INFO : New size: %lld bytes, modified time : %s \n",
                    (long long) w->attr.st_size, time_str);
        }
    }

    // Reset timeout_count
    timeout_count = 0;
}

/*
 * Libev ev_stat and ev_timer watcher example
 * 1) set stat watcher and timer watcher
 *    minitoring if file attribute has been changed, ex. mtime, size ....
 *    then trigger file_changed_cb()
 * 2) set ev_timer watcher and trigger timeout_cb() after timeout
 *    has been reached
 */

int main(int argc, char * argv[]) {

    if ( argc != 2 ) {
        printf("Usage : %s <file name>\n", argv[0]);
        printf("Example : \n");
        printf("          %s my_test.log\n", argv[0]);
        return -1;
    }

    // 1. declare event loop context EV_DEFAULT
    struct ev_loop *loop = EV_DEFAULT;

    /* 2. Initialize the stat watcher:
     *    ev_stat_init(ev,cb,path,interval)
     *    - &file_watcher: pointer to the struct
     *    - file_changed_cb: callback function
     *    - "test.txt": the literal file path to target
     *    - 0.0: interval hint in seconds (0.0 lets libev pick the fastest native method, like inotify)
     */

    ev_stat_init(&file_watcher, file_changed_cb, argv[1], 0.0);

    /*
     * 3. Initialize the timer:
     *    ev_timer_init(watcher, callback, after, repeat)
     *    - after: 1.0 second delay before the first trigger
     *    - repeat: 2.0 seconds interval for subsequent triggers
     *      To make a timer run only once, set the repeat parameter to 0.0.
     */

    ev_timer_init(&timeout_watcher, timeout_cb, 10.0, 10.0);

    // 4. Register and activate the watcher inside your loop
    ev_stat_start(loop, &file_watcher);
    ev_timer_start(loop, &timeout_watcher);
    printf("INFO : Start Watching file update event for <%s> \n", argv[1]);

    // 5. Start the execution loop
    ev_run(loop, 0);

    return 0;
}
