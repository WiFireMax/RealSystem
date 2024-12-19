#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define QUEUE_NAME "/my_queue"
#define MAX_MSG_SIZE 256

int main() {
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;

    // Unlink the queue if it already exists
    if (mq_unlink(QUEUE_NAME) == -1 && errno != ENOENT) {
        perror("mq_unlink");
        exit(EXIT_FAILURE);
    }

    // Create the message queue
    mqd_t mqdes = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0600, &attr);
    if (mqdes == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // Send a message
    char message[] = "Hello from sender!";
    if (mq_send(mqdes, message, strlen(message), 0) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }

    // Close the message queue
    mq_close(mqdes);

    return 0;
}
