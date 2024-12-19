#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LOOP_COUNT 30
#define SLEEP_TIME 1

void working(int tid);
void *server(void *arg);
void *t1(void *arg);
void *t2(void *arg);
