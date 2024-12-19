#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>

#define NumThreads      4       // позже установите значение 16

volatile int     var1;
volatile int     var2;
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

void    *update_thread (void *);
char    *progname = "nomutex";

int main ()
{
    pthread_t           threadID [NumThreads];  // хранит ID потоков
    pthread_attr_t      attrib;                 // атрибуты планирования
    struct sched_param  param;                  // для установки приоритета
    int                 i, policy;
    setvbuf (stdout, NULL, _IOLBF, 0);
    var1 = var2 = 0;        /* инициализация известных */
    printf ("%s:  starting; creating threads\n", progname);

    pthread_attr_init (&attrib);
    pthread_getschedparam (pthread_self(), &policy, &param);
    pthread_attr_setschedpolicy (&attrib, SCHED_RR);
    param.sched_priority -= 2;        // Снизить приоритет на 2 уровня
    pthread_attr_setschedparam (&attrib, &param);

    // Создаем потоки. С каждым завершением вызова pthread_create запускается поток.
    for (i = 0; i < NumThreads; i++) {
        pthread_create (&threadID [i], &attrib, &update_thread, (void *) (long)i);
    }

    sleep (20);
    printf ("%s:  stopping; cancelling threads\n", progname);
    for (i = 0; i < NumThreads; i++) {
      pthread_cancel (threadID [i]);
    }
    printf ("%s:  all done, var1 is %d, var2 is %d\n", progname, var1, var2);
    fflush (stdout);
    sleep (1);
    exit (0);
}

/*
 *  Текущий поток.
 *  The thread ensures that var1 == var2.  If this is not the
 *  case, the thread sets var1 = var2, and prints a message.
 *  Var1 and Var2 are incremented.
 *  Looking at the source, if there were no "synchronization" problems,
 *  then var1 would always be equal to var2.  Run this program and see
 *  what the actual result is...
*/

void do_work()
{
    static int var3;
    var3++;
    /* For faster/slower processors, may need to tune this program by
     * modifying the frequency of this printf -- add/remove a 0
     */
    if ( !(var3 % 10000000) ) 
        printf ("%s: thread %lu did some work\n", progname, pthread_self());
}

void *update_thread (void *i)
{
    while (1) {
        pthread_mutex_lock(&mutex);
        if (var1 != var2) {
            printf ("%s:  thread %lu, var1 (%d) is not equal to var2 (%d)!\n",
                    progname, pthread_self(), var1, var2);
            var1 = var2;
        }
        var1++;
        var2++;
        pthread_mutex_unlock(&mutex);

        /* do some work here */
        do_work(); 
    }
    return (NULL);
}
