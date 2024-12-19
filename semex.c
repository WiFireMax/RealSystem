#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

sem_t   *mySemaphore;
void    *producer (void *);
void    *consumer (void *);
char    *progname = "semex";

#define SEM_NAME "/Semex"

int main ()
{
    int     i;
    setvbuf (stdout, NULL, _IOLBF, 0);
    pthread_t consumerThreads[5];
    pthread_t producerThread;

    // Create a named semaphore
    mySemaphore = sem_open (SEM_NAME, O_CREAT | O_EXCL, S_IRWXU, 0);
    if (mySemaphore == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    for (i = 0; i < 5; i++) {
        // Create consumer threads
        int *threadArg = malloc(sizeof(int));
        *threadArg = i;
        pthread_create (&consumerThreads[i], NULL, consumer, threadArg);
    }

    // Create producer thread
    int *producerArg = malloc(sizeof(int));
    *producerArg = 1;
    pthread_create (&producerThread, NULL, producer, producerArg);

    sleep (20);     // let the threads run
    printf ("%s:  main, exiting\n", progname);
    return 0;
}

void *producer (void *i)
{
    while (1) {
        sleep (1);
        printf ("%s:  (producer %ld), posted semaphore\n", progname, (long)i);
        if (sem_post (mySemaphore) == -1) {
            perror("sem_post");
            exit(1);
        }
    }
    return (NULL);
}

void *consumer (void *i)
{
    while (1) {
        if (sem_wait (mySemaphore) == -1) {
            perror("sem_wait");
            exit(1);
        }
        printf ("%s:  (consumer %ld) got semaphore\n", progname, (long)i);
        free(i); // Free the allocated memory for the thread argument
    }
    return (NULL);
}
