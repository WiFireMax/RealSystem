#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <stdlib.h> // Include stdlib.h for rand() and srand()
#include <time.h>   // Include time.h for seeding with current time

volatile int        state;          // which state we are in
pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t      cond  = PTHREAD_COND_INITIALIZER;
void    *state_0 (void *);
void    *state_1 (void *);
void    *state_2 (void *);
void    *state_3 (void *);
char    *progname = "condvar";

int main ()
{
    setvbuf (stdout, NULL, _IOLBF, 0);
    state = 0;
    pthread_t threads[4];

    // Seed the random number generator with the current time
    srand(time(NULL));

    pthread_create (&threads[0], NULL, state_0, NULL);
    pthread_create (&threads[1], NULL, state_1, NULL);
    pthread_create (&threads[2], NULL, state_2, NULL);
    pthread_create (&threads[3], NULL, state_3, NULL);

    sleep (20); 
    printf ("%s:  main, exiting\n", progname);
    return 0;
}

/*
 *  Обработчик состояния 0
*/

void *state_0 (void *arg)
{
    while (1) {
        pthread_mutex_lock (&mutex);
        while (state != 0) {
            pthread_cond_wait (&cond, &mutex);
        }
        printf ("%s:  transit 0 -> 1\n", progname);
        state = 1;
        pthread_cond_signal (&cond);
        pthread_mutex_unlock (&mutex);
        usleep(100000); // Replace delay(100) with usleep(100000) for 100ms delay
    }
    return (NULL);
}

/*
 *  Обработчик состояния 1
*/

void *state_1 (void *arg)
{
    int internal_var;

    while (1) {
        pthread_mutex_lock (&mutex);
        while (state != 1) {
            pthread_cond_wait (&cond, &mutex);
        }
        internal_var = rand() % 2; // Generate a random even or odd number
        printf ("%s:  state 1, internal_var is %d\n", progname, internal_var);
        if (internal_var == 0) { // Even
            printf ("%s:  transit 1 -> 2\n", progname);
            state = 2;
        } else { // Odd
            printf ("%s:  transit 1 -> 3\n", progname);
            state = 3;
        }
        pthread_cond_signal (&cond);
        pthread_mutex_unlock (&mutex);
    }
    return (NULL);
}

/*
 *  Обработчик состояния 2
*/

void *state_2 (void *arg)
{
    while (1) {
        pthread_mutex_lock (&mutex);
        while (state != 2) {
            pthread_cond_wait (&cond, &mutex);
        }
        printf ("%s:  transit 2 -> 0\n", progname);
        state = 0;
        pthread_cond_signal (&cond);
        pthread_mutex_unlock (&mutex);
        usleep(100000); // Replace delay(100) with usleep(100000) for 100ms delay
    }
    return (NULL);
}

/*
 *  Обработчик состояния 3
*/

void *state_3 (void *arg)
{
    while (1) {
        pthread_mutex_lock (&mutex);
        while (state != 3) {
            pthread_cond_wait (&cond, &mutex);
        }
        printf ("%s:  transit 3 -> 0\n", progname);
        state = 0;
        pthread_cond_signal (&cond);
        pthread_mutex_unlock (&mutex);
        usleep(100000); // Replace delay(100) with usleep(100000) for 100ms delay
    }
    return (NULL);
}
