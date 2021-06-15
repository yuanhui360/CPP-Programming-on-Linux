#include <iostream>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define QUEUE_NAME  "/my_queue" /* Queue name. */
#define QUEUE_PERMS ((int)(0644))
#define QUEUE_MAXMSG  16 /* Maximum number of messages. */
#define QUEUE_MSGSIZE 1024 /* Length of message. */
#define QUEUE_ATTR_INITIALIZER ((struct mq_attr){0, QUEUE_MAXMSG, QUEUE_MSGSIZE, 0, {0}})

/* The consumer is faster than the publisher. */
#define QUEUE_POLL_CONSUMER ((struct timespec){2, 500000000})
#define QUEUE_POLL_PUBLISHER ((struct timespec){5, 0})

#define QUEUE_MAX_PRIO ((int)(9))


int main (int argc, char *argv[])
{

  struct mq_attr attr;          // To store queue attributes
  mqd_t mqdes;                  // Message queue descriptors
  char buf[QUEUE_MSGSIZE + 1];           // A good-sized buffer
  unsigned int prio;            // Priority
  std::string  inMgs;

    // First we need to set up the attribute structure
  attr.mq_maxmsg = QUEUE_MAXMSG;
  attr.mq_msgsize = QUEUE_MSGSIZE;
  attr.mq_flags = 0;
  prio = 0;
  memset(buf, '\0', sizeof(buf));

  // Open a queue with the attribute structure
  mqdes = mq_open (QUEUE_NAME, O_CREAT | O_RDWR | O_NONBLOCK, QUEUE_PERMS, &attr);
  if ( mqdes == -1 ) {
      perror ("mq_open()");
      exit(1);
  }

  for(;;) {
      std::cin.getline(buf, sizeof(buf));
      if ( !strcmp(buf, "end") ) {
         break;
      }
      if (mq_send (mqdes, buf, strlen(buf), prio) == -1)
          perror ("mq_send()");
  }

  // Close all open message queue descriptors
  if (mq_send (mqdes, buf, strlen(buf), prio) == -1)
      perror ("mq_send()");
  mq_close (mqdes);
  return 0;
}
