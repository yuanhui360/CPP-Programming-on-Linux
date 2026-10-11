#include <iostream>
#include <fstream>
#include <csignal>
#include <ev++.h>             // Libev C++ Wrapper Class
#include <sys/stat.h>
#include <ctime>
#include <iomanip>            // For std::put_time()

class FileMonitorApp {
public:
    // Constructor of class pass file_path string
    FileMonitorApp(const std::string &path) : filepath(path) {
        // 1. Attach member callback for ev::stat, ev::timer and ev::sig
        // template set:
        // w->set<class, &class::method> (object *)

        stat_watcher.set<FileMonitorApp, &FileMonitorApp::stat_cb>(this);
        timer_watcher.set<FileMonitorApp, &FileMonitorApp::timer_cb>(this);
        sig_watcher.set<FileMonitorApp, &FileMonitorApp::signal_cb>(this);

        // 2 Start monitor watchers

        stat_watcher.start(filepath.c_str(), 0.0);        // Here 0.0 lets libev choose optimal default
        timer_watcher.start(20.0, 10.0);                  // after first trigger timer, repeat interval
        sig_watcher.start(SIGINT);                        // Signal [SIGINT] Catch Ctrl+C signal

        std::cout << "INFO : [FileMonitorApp] Start Monitoring update for file <"
                  << filepath << ">" << std::endl;
    }

private:
    // Member Variable
    ev::stat     stat_watcher;         // ev_stat
    ev::timer    timer_watcher;        // ev_timer
    ev::sig      sig_watcher;          // ev_signal
    std::string  filepath;
    int timer_count = 0;

    // Create 3 private member callback method

    // stat watcher callback method
    void stat_cb(ev::stat &w, int revents) {
        // 1. Reset timer_count back to 0
        timer_count = 0;

        // 2. Print callback has been triggered info
        std::cout << "INFO : [FILE  EVENT] Revents(" << revents
                  << ") Modification detected for file : " << filepath << std::endl;
        if ( revents & ev::STAT ) {
            // 3. check if the file actually exist
            if ( w.attr.st_nlink == 0 ) {
                std::cout << "ERROR : [FILE  EVENT] File was deleted or unreachable : "
                          << filepath << std::endl;
                return;
            }

            // 4. print old size and new size
            std::cout << "     - Size Changed: " << w.prev.st_size << " bytes -> "
                      << w.attr.st_size << " bytes ";

            // 5. print modification date/time change
            if ( w.attr.st_size == 0 ) {
                std::cout << " - Old Modified time (mtime): "     // Date Format: YYYY-MM-DD HH:MM:SS
                << std::put_time(std::localtime(&w.prev.st_mtime), "%Y-%m-%d %H:%M:%S") << std::endl;
            } else {
                std::cout << " - New Modified time (mtime): "     // Date Format: YYYY-MM-DD HH:MM:SS
                << std::put_time(std::localtime(&w.attr.st_mtime), "%Y-%m-%d %H:%M:%S") << std::endl;
            }

            // 6. If text was appended, read from file and print new content
            if ( w.attr.st_size > w.prev.st_size ) {
                print_new_content(w.prev.st_size);
            }
        }
    }

    void print_new_content(off_t offset) {
        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        if (file) {
            file.seekg(offset);
            std::string new_data((std::istreambuf_iterator<char>(file)),
                                  std::istreambuf_iterator<char>());
            std::cout << "  ----- New Content Append -----\n" << new_data;

        }
    }

    // timer watcher callback method
    void timer_cb(ev::timer &w, int revents) {

        // 1. timer count ++
        timer_count++;

        // 2. Print timer watcher callback method has been triggered
        std::cout << "INFO : [TIMER EVENT]: Revents(" << revents << ") Repeat("
                  << w.repeat << ") seconds(s) timeout count ("
                  << timer_count << ") ..." << std::endl;
        // 3. if timer count >= 5 stop timer watcher
        if ( timer_count >= 5 ) {
            std::cout << "INFO : Timer reached limit, Stopping timer watcher ..." << std::endl;
            w.stop();       // Stop timer watcher manually
        }
    }

    // signal watcher callback method
    void signal_cb(ev::sig &w, int revents) {
        // 1. Print INFO signal callback method has been invoked
        std::cout << "INFO : [SIGNAL EVENT] Revents(" << revents
                  << ") Stopping file monitor App and break event loop ..." << std::endl;

        // 2. stop event loop
        w.loop.break_loop(ev::ALL);              // ev::ONE or ev::ALL
    }

};

int main(int argc, char * argv[]) {

    if ( argc != 2 ) {
        printf("Usage : %s <file name>\n", argv[0]);
        printf("Example : \n");
        printf("          %s my_test.log\n", argv[0]);
        return -1;
    }

    std::string test_filename = std::string(argv[1]);

    // Ensure a target file exists to watch
    {
        std::ofstream initial_file(test_filename, std::ios::app);
    }

    // 1) Instantiating FileMonitorApp object or creating appinstance
    FileMonitorApp app(test_filename);

    // 2) Get the default event loop and start running
    ev::default_loop   loop;
    loop.run(0);

    std::cout << "INFO : [FileMonitorApp] Event loop ended successfully." << std::endl;
    return 0;
}
