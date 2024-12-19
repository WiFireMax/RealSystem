#include "working.h"
#include <pthread.h>

int main(int argc, char *argv[]) {
    pthread_t pthread_server;
    pthread_t pthread_t1;
    pthread_t pthread_t2;
    int tid_server = 0;
    int tid_t1 = 1;
    int tid_t2 = 2;

    pthread_create(&pthread_server, NULL, server, &tid_server);
    pthread_create(&pthread_t1, NULL, t1, &tid_t1);
    pthread_create(&pthread_t2, NULL, t2, &tid_t2);

    pthread_join(pthread_server, NULL);
    pthread_join(pthread_t1, NULL);
    pthread_join(pthread_t2, NULL);

    return EXIT_SUCCESS;
}
