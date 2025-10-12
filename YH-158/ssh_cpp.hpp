#ifndef _SSH_CPP_HPP_
#define _SSH_CPP_HPP_

#include <stdlib.h>
#include <stdarg.h>
#include <termios.h>

#include <libssh/libssh.h>
#include <libssh/server.h>

#define SSH_NO_CPP_EXCEPTIONS
#include <libssh/libsshpp.hpp>

namespace ssh {

class my_session : public Session {
public:

    /*
     * https://linux.die.net/man/3/cfmakeraw
     * Setting the local terminal to "raw" mode with the cfmakeraw(3) function.
     * cfmakeraw() is a standard function under Linux:
     * 1) the echo from the keys pressed should be done by the remote side.
     * 2) User's input should not be sent once "Enter" key is pressed,
     *    but immediately after each key is pressed.
     */

    void cfmakeraw(struct termios *termios_p)
    {
        termios_p->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
        termios_p->c_oflag &= ~OPOST;
        termios_p->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
        termios_p->c_cflag &= ~(CSIZE|PARENB);
        termios_p->c_cflag |= CS8;
    }

    /*
     * Linux function determines whether a key has been pressed.
     */

    int kbhit()
    {
        struct timeval tv = { 0L, 0L };
        fd_set fds;

        FD_ZERO(&fds);
        FD_SET(0, &fds);     // <-- 0: stdin  (standard input)

        return select(1, &fds, NULL, NULL, &tv);
    }

protected:
private:
};

}  // End namespace ssh

#endif /* _SSH_CPP_HPP_ */
