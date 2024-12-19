#include "working.h"
#include <unistd.h>

void working(int tid) {
    int i;
    for (i = 0; i < LOOP_COUNT; i++) {
        printf("Server is working for %d - %dth start\n", tid, i);
        sleep(SLEEP_TIME);
        printf("Server is working for %d - %dth end\n", tid, i);
    }
    printf("Server finished all work for %d\n", tid);
}

void *server(void *arg) {
    working(*(int *)arg);
    return NULL;
}

void *t1(void *arg) {
    working(*(int *)arg);
    return NULL;
}

void *t2(void *arg) {
    working(*(int *)arg);
    return NULL;
}
