#include "working.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main(int argc, char *argv[]) {
    // Initialize mutex attributes
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&mutex, &attr);

    // Create threads
    pthread_t high_priority_thread_id;
    pthread_t low_priority_thread_id;
    pthread_t medium_priority_thread_id;

    int high_tid = 3;
    int low_tid = 1;

    // Set thread priorities
    struct sched_param sched_high;
    sched_high.sched_priority = 30; // High priority
    struct sched_param sched_low;
    sched_low.sched_priority = 10; // Low priority
    struct sched_param sched_medium;
    sched_medium.sched_priority = 20; // Medium priority

    pthread_attr_t attr_high, attr_low, attr_medium;
    pthread_attr_init(&attr_high);
    pthread_attr_init(&attr_low);
    pthread_attr_init(&attr_medium);

    pthread_attr_setschedparam(&attr_high, &sched_high);
    pthread_attr_setschedparam(&attr_low, &sched_low);
    pthread_attr_setschedparam(&attr_medium, &sched_medium);

    // Create and start threads
    pthread_create(&low_priority_thread_id, &attr_low, t1, &low_tid);
    pthread_create(&high_priority_thread_id, &attr_high, t2, &high_tid);
    pthread_create(&medium_priority_thread_id, &attr_medium, server, &high_tid);

    // Wait for threads to finish
    pthread_join(low_priority_thread_id, NULL);
    pthread_join(high_priority_thread_id, NULL);
    pthread_join(medium_priority_thread_id, NULL);

    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&attr);

    return EXIT_SUCCESS;
}
