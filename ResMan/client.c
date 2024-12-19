#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>

#define QUEUE_NAME "/my_queue"
#define MAX_MSG_SIZE 256

int main() {
    // Open the message queue
    mqd_t mqdes = mq_open(QUEUE_NAME, O_RDWR);
    if (mqdes == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // Get the message queue attributes
    struct mq_attr attr;
    mq_getattr(mqdes, &attr);

    // Receive a message
    char buffer[attr.mq_msgsize];
    unsigned int prio;
    ssize_t bytes_received = mq_receive(mqdes, buffer, attr.mq_msgsize, &prio);
    if (bytes_received == -1) {
        perror("mq_receive");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0'; // Null-terminate the string

    printf("Received message: %s\n", buffer);

    // Close the message queue
    mq_close(mqdes);

    // Unlink the message queue
    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}
